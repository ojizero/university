import re
import json
import requests
from daemon import Daemon
from serial import Serial
from config import params
from Adafruit_CharLCD import Adafruit_CharLCD as AdaLcd

class TheDaemon(Daemon):
	def run(self):
		# LCD object
		lcd = AdaLcd(**params['_lcd_pns'])
		lcd.clear()

		URL = params['_url']
		client = requests.session()

		# Retrieve the CSRF token first
		get = client.get(URL + '/login')  # sets cookie

		login_data = {
			'id'       : params['_permit'],
			'password' : params['_password'],
			'_token'   : re.compile('\"_token\".*value=\"(?P<Value>\w*)\"\>')
						   .search(get.text).group('Value') # get the token from text
		}
		client.post(URL+'/login', data=login_data, cookies=client.cookies)

		payment_count = 0
		lcd_str       = 'Paid:%d. Rem:%d\n%s.'

		reader = Serial(port=params['_port'])
		""" TheLoop """
		while True:
			read_val = reader.read(params['_size'])

			lcd.clear()
			lcd.message(lcd_str%(payment_count, params['_mas_passengers'] - payment_count, '....'))

			uid_val  = json.loads(read_val[:-2].decode())['UID']
			ret      = json.loads(client.get(URL+'/transfer/%s/%s/%s'%(uid_val, params['_permit'], params['_cost']), cookies=client.cookies).text)

			if ret['_status'] == 200:
				payment_count += 1

			lcd.clear()
			lcd.message(lcd_str%(payment_count, params['_mas_passengers'] - payment_count, ret['_description']))


if __name__ == '__main__':
	TheDaemon('/dev/null').start()
