import re
import json
import requests
from daemon import Daemon
from serial import Serial
from config import params

class TheDaemon(Daemon):
	def run(self):
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

		# send kill signal to server
		client.post(URL+'/login', data=login_data, cookies=client.cookies)

		reader = Serial(port=params['_port'])
		""" TheLoop """
		while True:
			read_val = reader.read(params['_size'])
			uid_val  = json.loads(read_val[:-2].decode())['UID']
			ret      = client.get(URL+'/transfer/%s/%s/%s'%(uid_val, params['_permit'], params['_cost']), cookies=client.cookies).text


if __name__ == '__main__':
	URL = params['_url']
	client = requests.session()

	# Retrieve the CSRF token first
	get = client.get(URL + '/login')  # sets cookie

	login_data = {
		'id'   : params['_permit'],
		'password' : params['_password'],
		'_token'   : re.compile('\"_token\".*value=\"(?P<Value>\w*)\"\>')
					   .search(get.text).group('Value') # get the token from text
	}

	# send kill signal to server
	client.post(URL+'/login', data=login_data, cookies=client.cookies)

	reader = Serial(port=params['_port'])
	""" TheLoop """
	while True:
		read_val = reader.read(params['_size'])
		uid_val  = json.loads(read_val[:-2].decode())['UID']
		print(client.get(URL+'/transfer/%s/%s/%s'%(uid_val, params['_permit'], params['_cost']), cookies=client.cookies).text)

