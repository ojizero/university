import requests

URL = 'http://localhost:8000/'

client = requests.session()

# Retrieve the CSRF token first
client.get(URL + 'login')  # sets cookie
csrftoken = client.cookies['XSRF-TOKEN']

login_data = dict(permit='123456', password='12345678', csrfmiddlewaretoken=csrftoken, next='/')
r = client.post(URL, data=login_data, headers=dict(Referer=(URL)))

res = client.get(URL + 'kill')

print(res)
