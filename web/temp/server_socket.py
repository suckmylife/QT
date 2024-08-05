import socket
import pymysql


########################################
### UDP Relay Server
########################################


class UDPRelayServer():
    def __init__(self, ip:str, port:int, buffer_size:int):
        # packet
        self.ip = ip 
        self.port = port

        self.u_socket = None
        self.buffer_size = buffer_size

        # DB
        self.connection = None
        


    def create_socket(self):
        """ create socket & bind """
        try:
            # Create socket
            if self.u_socket == None:
                self.u_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
                self.u_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # port 사용중 에러를 막기 위해 port 재지정
            print('Socket Created!')
        except:
            error_code = socket.error()
            print(f"Couldn't create socket : {error_code}")
        

        try:
            # Bind the source address
            self.u_socket.bind((self.ip, self.port))
            print('Socket Bind success!')
        except:
            error_code = socket.error()
            print(f"Couldn't create socket : {error_code}")
        



    def create_db_connection(self, host, port, user, password, db_name, db_table):
        """ Connect to MySQL Server """
        self.mysql_host = host
        self.mysql_port = port
        self.mysql_user = user
        self.mysql_password = password
        self.mysql_db_name = db_name
        self.mysql_db_table = db_table

        try: 
            self.connection = pymysql.connect(
                host=self.mysql_host,
                port=self.mysql_port,
                user=self.mysql_user,
                passwd=self.mysql_password,
                db=self.mysql_db_name
            )
            print('MySQL connect success!')
        except:
            raise ConnectionError('MySQL connect failed!')


    def insert_db(self, device_id, device_ip, data):
        """ Insert Sensor Data to MySQL Server """
        cursor = self.connection.cursor()
        
        sql = f'INSERT INTO {self.mysql_db_table}(device_id, ip, data) VALUES({str(device_id), str(device_ip), str(data)})'

        try:
            cursor.execute(sql)
            self.connection.commit()
            
            return True 
        except:
            return False


    def relay_to_clients(self, data:bytes, address:tuple):
        """ Relay packet to clients """
        try:
            self.u_socket.sendto(data, address)
            print(f'SUCCESS : UDP Send Packet to Clients -> {address}')
        except:
            print(f'FAIL : UDP Send Packet to Clients -> {address}')
        

    def parsing(self):
        """ 솔님 """

        pass


    def run(self):
        """ Run UDPServerSocket """
        print('UDP server up and listening')

        while True:
            data, address = self.u_socket.recvfrom(self.buffer_size)
            
            msg = f'Data From Client : {data}'
            client_ip = f'Client IP Address : {address}'

            print(msg)
            print(client_ip)

            self.parsing()
            self.insert_db()

            pass
    


    def close(self):
        self.u_socket.close()
        print('UDP socket closed')



# run
udp_relay_server = UDPRelayServer('0.0.0.0', 50000, 1024)
udp_relay_server.create_socket()
udp_relay_server.create_db_connection('172.47.20.213', 3306, 'test_user', '^Neoable', 'test_db', 'test_table')
udp_relay_server.run()