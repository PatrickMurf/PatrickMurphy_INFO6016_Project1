#pragma once
#include "string"
#include "buffer.h"

#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

struct PacketHeader
{
	uint32_t packetSize;
	uint32_t messageType;
};

struct ChatMessage
{
	PacketHeader header;
	uint32_t messageLength;
	std::string message;
};

void sendMessagePacket(SOCKET serverSocket, std::string userName, std::string messageInput)
{
	ChatMessage message;
	message.message = userName + messageInput;
	message.messageLength = message.message.length();
	message.header.messageType = 1;
	message.header.packetSize = message.messageLength + sizeof(message.messageLength) + sizeof(message.header.messageType) + sizeof(message.header.packetSize);

	const int bufSize = 512;
	Buffer buffer(bufSize);

	buffer.WriteUInt32LE(message.header.packetSize);
	buffer.WriteUInt32LE(message.header.messageType);
	buffer.WriteUInt32LE(message.messageLength);
	buffer.WriteString(message.message);

	send(serverSocket, (const char*)(&buffer.m_BufferData[0]), message.header.packetSize, 0);
}