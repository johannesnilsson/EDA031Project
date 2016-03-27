
#include "messagehandler.h"
#include "connection.h"
#include "protocol.h"

#include <string>
#include <iostream>

using namespace std;

MessageHandler::MessageHandler(){}

/**
 * Create a message handler.
 * 
 * @param conn
 *            The connection to use messages
 */
MessageHandler::MessageHandler(Connection& conn) : myConn(conn) {}

void MessageHandler::changeConnection(Connection& conn){
	myConn = conn;
}	
/**
 * Transmit a code (a constant from the Protocol class).
 * 
 * @param code
 *            The code to transmit.
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendCode(int code){
	sendByte(code);
}

/**
 * Transmit an int value, according to the protocol.
 * 
 * @param value
 *            The value to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendInt(int value){
	sendByte((value >> 24) & 0xFF);
	sendByte((value >> 16) & 0xFF);
	sendByte((value >> 8) & 0xFF);
	sendByte(value & 0xFF);
}

/**
 * Transmit an int parameter, according to the protocol.
 * 
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendIntParameter(int param){
	sendCode(Protocol::PAR_NUM);
	sendInt(param);
}

/**
 * Transmit a string parameter, according to the protocol.
 * 
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendStringParameter(string param){
	sendCode(Protocol::PAR_STRING);
	sendInt(param.length());
	for (unsigned int i = 0; i < param.length(); ++i) {
		sendByte(param.at(i));
	}
}

/**
 * Receive a command code or an error code from the server.
 * 
 * @return The code
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvCode(){
	int code = recvByte();
	return code;
}

/**
 * Receive an int value from the server.
 * 
 * @return The int value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvInt(){
	int b1 = recvByte();
	int b2 = recvByte();
	int b3 = recvByte();
	int b4 = recvByte();
	
	return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

/**
 * Receive an int parameter from the server.
 * 
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvIntParameter(){
	int code = recvCode();
	if (code != Protocol::PAR_NUM) {
		//throw new ProtocolViolationException("Receive numeric parameter",
				//Protocol::PAR_NUM, code);
	}
	return recvInt();
}

/**
 * Receive a string parameter from the server.
 * 
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */
string MessageHandler::recvStringParameter(){
	int code = recvCode();
	if (code != Protocol::PAR_STRING) {
		//throw new ProtocolViolationException("Receive string parameter",
				//Protocol::PAR_STRING, code);
	}
	int n = recvInt();
	if (n < 0) {
		//throw new ProtocolViolationException("Receive string parameter",
				//"Number of characters < 0");
	}
	//StringBuffer result = new StringBuffer(n);
	string result = "";
	for (int i = 1; i <= n; i++) {
		char ch = (char) myConn.read();
		result += ch; 
	}
	
	return result; //.toString();
}


void MessageHandler::sendByte(int code){
	//try {
		//cout << "Running myConn.write in sendByte with b: " << code << endl;
		myConn.write((char) code);
	//} catch (java.io.IOException e) {
	//	throw new ConnectionClosedException();
	//}
}

int MessageHandler::recvByte(){
	int code = myConn.read();
	//if (code == Connection::CONNECTION_CLOSED) {
		//throw new ConnectionClosedException();
	//}
	return code;
}