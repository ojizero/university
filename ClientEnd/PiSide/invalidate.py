import re
import sys
import json
import requests
from config import params as information

# information = None
# with open(sys.argv[1], 'r') as info:
# 	information = json.loads(info.read())

# if not information:
# 	exit(-1)

URL = information['_url']
client = requests.session()

# Retrieve the CSRF token first
get = client.get(URL + '/login')  # sets cookie

login_data = {
	'id'       : information['_permit'],
	'password' : information['_password'],
	'_token'   : re.compile('\"_token\".*value=\"(?P<Value>\w*)\"\>')
				   .search(get.text).group('Value') # get the token from text
}

# send kill signal to server
client.post(URL+'/login', data=login_data, cookies=client.cookies)

status = json.loads(client.get(URL+'/maintenance', cookies=client.cookies).text)
# print(status['status'])
if status['status']:
	print('true')
else:
	client.get(URL + '/kill')
