#!/usr/bin/env python3

"""
Ameer Alkam - 1120217
"""

from threading import Thread, Lock
from socket import socket, AF_INET, SOCK_STREAM
from os import listdir, path
from time import sleep


# /**
#  * TODO Network assignment
#  * FIXME threaded work, added persistent mode in server
#  * counter in the class,
#  * reset every connection serviced,
#  * thread counts it down,
#  **/


class ServerHandler:
	def __init__(self):
		self.req_count = 0

		# list of files in the server
		file_list = listdir('./')  # holds files in directory

		# remove any python source code (this current file)
		i = 0
		while i < len(file_list):
			tmp = file_list[i]
			if tmp[len(tmp) - 2:] == 'py':
				file_list.remove(tmp)
			else:
				i += 1

		# hold files and the number of requests for them
		self.file_count = dict()
		for x in file_list:
			self.file_count[x] = 0

		self.listlock = Lock()
		self.lock = Lock()

	def server(self, command, connection, arg=None):
		"""
		:param command: operation requested
		:type command: str

		:param connection: socket of the connection
		:type connection: socket

		:param arg: extra arguments (used for file names)
		:type arg: str
		"""

		if command == 'list':
			self.list_files(connection)
		elif command == 'stats':
			self.send_stats(connection)
		elif command == 'get':
			if arg is None:
				connection.close()
				return

			self.send_file(connection, arg)
		elif command == 'put':
			if arg is None:
				connection.close()
				return

			self.recv_file(connection, arg)
		else:
			pass

		connection.close()

	def send_file(self, connection, file_path):
		self.req_count += 1
		if file_path in self.file_count:
			with open(file_path, 'rb') as f:
				connection.sendfile(f)
			self.file_count[file_path] += 1
		else:
			return

	def recv_file(self, connection, file_name):
		with self.lock:
			if path.exists(file_name):
				connection.send(b'nok')
				return
			else:
				connection.send(b'ok')
				# create an empty file, to make sure any other immediate test for the existence of the files won't fail
				with open(file_name, 'w') as f:
					pass

		data = (connection.recv(buffer_size)).decode()
		with open(file_name, 'w') as f:
			while data:
				f.write(data)
				data = (connection.recv(buffer_size)).decode()

			# self.file_count[file_name] = 0

	def list_files(self, connection):
		# check if lock isn't being used (file list isn't being updated)
		self.listlock.acquire()
		# unlock immediately after
		self.listlock.release()

		for x in self.file_count:
			s = str(x) + ';'
			connection.send(s.encode())

	def send_stats(self, connection):
		# check if lock isn't being used (file list isn't being updated)
		self.listlock.acquire()
		# unlock immediately after
		self.listlock.release()

		for x, y in self.file_count.items():
			s = str(x) + ' : ' + str(y)
			connection.send(s.encode() + b';')
		connection.send(('\nTotal requests : ' + str(self.req_count)).encode())

	def updater(self):
		while True:
			sleep(10)
			with self.listlock:
				self.file_count = {key: value for key, value in self.file_count.items() if path.exists(key)}


if __name__ == '__main__':
	# serves the clients
	serve = ServerHandler()

	# server socket
	server_socket = socket(AF_INET, SOCK_STREAM)
	server_port = 64601

	server_socket.bind(('', server_port))
	server_socket.listen()

	try:
		# background thread to keep the list of files up to date
		updater = Thread(target=serve.updater)
		updater.daemon = True
		updater.start()

		while True:
			print('waiting connections ...')
			command = None
			connection = None
			arg = None
			buffer_size = 1024

			# connection with client socket
			connection, info = server_socket.accept()
			print('connection established {0}'.format(str(info)))
			# receive command
			op = (connection.recv(buffer_size).decode()).split(maxsplit=1)

			command = op[0]
			if len(op) == 2:
				arg = op[1]

			print('serving connection {0}\n\n'.format(str(info)))

			# create new thread and let it manage serving the current client in the background
			th = Thread(target=serve.server, args=(command, connection, arg))
			th.daemon = True  # runs in background, gets killed when main program exists
			th.start()

	except KeyboardInterrupt or Exception:
		print('\n\n\n...\n\ninterrupted ...')
		server_socket.close()
		print('closed server socket ...')
		exit(0)
