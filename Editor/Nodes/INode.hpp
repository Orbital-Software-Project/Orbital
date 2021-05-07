#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>


enum class INodeDataType {
	INodeDataType_Unknown,
	Type_OpenCV_VideoCapture,
	Type_OpenCV_Mat,
	Type_Int,
};

enum class NodeSocketIOType {
	Unknown,
	Output,
	Input
};

enum class INodeType {
	Node_None,
	Node_Clip,
	Node_Video,
	Node_Color,
	Node_FeatureTrack
};

struct SocketData {

	struct {
		cv::VideoCapture Video;
		int FrameCount = 0;
		std::string FilePath = "";
		cv::Mat MatData = cv::Mat();
	} NodeType_Video;


	int IntData = 0;

	std::string StrData = "";
};

struct NodeSocket {

	NodeSocket() {}

	NodeSocket(INodeDataType data_type, int node_id, int socket_id, NodeSocketIOType io_type) : Type(data_type), NodeId(node_id), SocketId(socket_id), IOType(io_type) {}


	~NodeSocket() {
		std::cout << "Destructor called" << std::endl;
	}

	NodeSocketIOType IOType = NodeSocketIOType::Unknown;

	INodeDataType Type = INodeDataType::INodeDataType_Unknown;

	int NodeId = 0;

	int SocketId = 0;

	std::shared_ptr<SocketData> Data = nullptr;
};


static unsigned int GlobalNodeId = 0;

class INode {

public:
	INode() {
		this->id = this->GetNextId();
	};

	virtual ~INode() {}

	virtual void OnRender() = 0;

	virtual bool OnRenderProperties() { return false; }

	std::vector<std::shared_ptr<NodeSocket>> GetOutputSockets() {
		std::vector<std::shared_ptr<NodeSocket>> sc;
		std::copy_if(
			this->io_sockets.begin(), 
			this->io_sockets.end(), 
			std::back_inserter(sc),
			[&](std::shared_ptr<NodeSocket> p_sc) { return p_sc->IOType == NodeSocketIOType::Output; }
		);

		return sc;
	}

	std::vector<std::shared_ptr<NodeSocket>> GetInputSockets() {
		std::vector<std::shared_ptr<NodeSocket>> sc;
		std::copy_if(
			this->io_sockets.begin(),
			this->io_sockets.end(),
			std::back_inserter(sc),
			[&](std::shared_ptr<NodeSocket> p_sc) {return p_sc->IOType == NodeSocketIOType::Input; });

		return sc;
	}

	int GetId() { 
		return this->id; 
	}

	virtual bool ComputeFrame(int frameIdx) {

		return true;
	}

	bool HasSocket(int socket_id) {
		for (auto socket : this->io_sockets) {
			if (socket->SocketId == socket_id) {
				return true;
			}
		}
		return false;
	}

	bool IsComputed() {
		return this->isComputed;
	}

	void Reset() {
		this->isComputed = false;
	}

	void WriteSocket(int socket_id, std::shared_ptr<SocketData> data) {
		for (auto& nc : this->io_sockets) {
			if (nc->SocketId == socket_id) {
				nc->Data = data;
			}
		}
	}

	std::shared_ptr<SocketData> ReadSocket(int socket_id) {
		for (auto& nc : this->io_sockets) {
			if (nc->SocketId == socket_id && 
				nc->Data.get() != nullptr) {

				return nc->Data;
			}
		}

		std::shared_ptr<SocketData> emptyData = std::make_shared<SocketData>();
		emptyData->NodeType_Video.MatData = cv::Mat();
		return emptyData;
	}

	INodeType Type = INodeType::Node_None;

	bool RequestCompute = false;

protected:
	std::shared_ptr<NodeSocket> RegisterInputSocket(INodeDataType data_type) {
		std::shared_ptr<NodeSocket> sc = std::make_shared<NodeSocket>(data_type, this->id, this->GetNextId(), NodeSocketIOType::Input);
		this->io_sockets.push_back(sc);
		return sc;
	}

	std::shared_ptr<NodeSocket> RegisterOutputSocket(INodeDataType data_type) {
		std::shared_ptr<NodeSocket> sc = std::make_shared<NodeSocket>(data_type, this->id, this->GetNextId(), NodeSocketIOType::Output);
		this->io_sockets.push_back(sc);
		return sc;
	}

	int GetNextId() { 
		return GlobalNodeId++; 
	}

	int id = 0;

	bool isComputed = false;

private:
	std::vector<std::shared_ptr<NodeSocket>>   io_sockets;

};