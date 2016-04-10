#!/usr/bin/env python3

"""
Ameer Alkam - 1120217
"""


class SocketHandler(object):
	def __init__(self, server_adrs, buffer_size=1024):
		"""
		:param server_adrs: destination address, a tuple of IPv4 and port
		:type server_adrs: (str, int)
		:param buffer_size: buffer size used in sending and receiving data
		:type buffer_size: int
		"""

		self.server_adrs = server_adrs
		self.buffer_size = buffer_size

	def send_file(self, file_path=None):
		"""
		:param file_path: path to the file being sent
		:type file_path: str
		:returns True -> success, False -> file already exists in server failure
		"""

		if file_path is None:
			return False

		from socket import socket, AF_INET, SOCK_STREAM

		connection = socket(AF_INET, SOCK_STREAM)
		connection.connect(self.server_adrs)

		# send command to server
		connection.send(('put {0}'.format(file_path)).encode())
		# wait server to accept command
		ok = connection.recv(self.buffer_size).decode()
		if ok == 'ok':
			with open(file_path, 'rb') as f:
				connection.sendfile(f)
			ret = True
		else:
			ret = False

		connection.close()
		return ret

	def recv_file(self, file_name=None):
		"""
		:param file_name: name of file to be requested
		:type file_name: str
		"""

		if file_name is None:
			return

		from socket import socket, AF_INET, SOCK_STREAM

		connection = socket(AF_INET, SOCK_STREAM)
		connection.connect(self.server_adrs)

		connection.send(('get {0}'.format(file_name)).encode())

		data = (connection.recv(self.buffer_size)).decode()
		with open(file_name, 'w') as f:
			while data:
				f.write(data)
				data = (connection.recv(self.buffer_size)).decode()

		connection.close()

	def list_files(self):
		"""
		:return: list of files on the server
		"""

		from socket import socket, AF_INET, SOCK_STREAM

		connection = socket(AF_INET, SOCK_STREAM)
		connection.connect(self.server_adrs)
		connection.send(b'list ')

		ls = ''
		data = (connection.recv(self.buffer_size)).decode()
		while data:
			ls += data
			data = (connection.recv(self.buffer_size)).decode()
		ls = ls.split(';')
		return ls

	def get_stats(self):
		"""
		:return: list of files along with the number of requests per each, with the total number of 'get' requests made
		"""

		from socket import socket, AF_INET, SOCK_STREAM

		connection = socket(AF_INET, SOCK_STREAM)
		connection.connect(self.server_adrs)
		connection.send(b'stats ')

		ls = ''
		data = (connection.recv(self.buffer_size)).decode()
		while data:
			ls += data
			data = (connection.recv(self.buffer_size)).decode()
		ls = ls.split(';')
		return ls


if __name__ == '__main__':
	# take IP address of host, default 'localhost'
	server_name = input('> Insert server name/IP [default:\'localhost\']: ') or 'localhost'

	# take PORT number of server host
	try:
		server_port = int(input('> Insert port number: '))
	except ValueError:
		server_port = ''

	while not server_port:
		try:
			server_port = int(input('> Please insert the port number! '))
		except ValueError:
			server_port = ''

	s = SocketHandler((server_name, server_port))

	print(
		'''\n\n\t\t* Insert command.
			- List to receive a list of available files
			- Get [file_name] to receive a file
			- Put [file_name] to send file
			- Stats to get request rates to files
			- Quit to close session.'''
	)

	while True:
		inp = str(input('>> ')).split(sep=' ', maxsplit=1)
		op = (inp[0]).lower()

		if len(inp) == 2:
			fi = inp[1]
		elif op == 'get' or op == 'put':
			print('Get/Put commands take a file name as well!')
			continue

		if op == 'quit':
			print('Good bye.')
			exit(0)
		elif op == 'list':
			ls = s.list_files()
			for x in ls:
				print(x)
		elif op == 'get':
			s.recv_file(fi)
		elif op == 'put':
			flag = s.send_file(fi)
			if flag:
				print('Transfer complete.')
			else:
				print('Transfer refused, server has file named {0}'.format(fi))
		elif op == 'stats':
			ls = s.get_stats()
			for x in ls:
				print(x)
		else:
			print('Invalid option.\n')
			continue
