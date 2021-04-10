#ifndef _CONSISTENT_HASH_
#define _CONSISTENT_HASH_
#include <memory>
#include <unordered_set>
#include <boost/optional.hpp>

namespace util
{
	template<typename T1, typename T2>
	class ConsistentHash
	{
	public:
		ConsistentHash() {}
		~ConsistentHash() {}
	public:
		struct Node
		{
		public:
			Node(T1 _identify, int _vNum = m_defaultVNum) :vNum(_vNum), identify(_identify) {}
			~Node()
			{

			}
		public:
			int	vNum;			//该物理节点的虚拟节点数
			T1	identify;		//节点标识,可以是ip，描述符，唯一表示等
		public:
			bool operator==(const Node& r) const
			{
				return r.identify == this->identify;
			}
		};
		using NodeType = Node;
	public:
		class NodeHash
		{
		public:
			std::size_t operator()(const Node& node) const
			{
				return std::hash<T2>{}(node.identify);
			}
		};
	public:
		void addNode(T1 Identifier, int vnum = m_defaultVNum)
		{
			static_assert(vnum > 0);
			Node node(Identifier, vnum);
			if (m_existNode.find(node) != m_existNode.end())
			{
				assert(0);
				return;
			}
			auto sharedNode = std::make_shared<Node>(node);
			for (auto idx = 0; idx < vnum; ++idx)
			{
				auto hcode = std::hash<std::string>{}(std::to_string(std::hash<T1>{}(Identifier)) + std::to_string(idx));
				m_nodeMap[hcode] = sharedNode;
			}
			m_existNode.insert(node);
		}
		void removeNode(T1 Identifier)
		{
			auto it = m_existNode.find(Identifier);
			if (it == m_existNode.end())
			{
				return;
			}
			for (auto idx = 0; idx < it->vNum; ++idx)
			{
				auto hcode = std::hash<std::string>{}(std::to_string(std::hash<T1>{}(Identifier)) + std::to_string(idx));
				m_nodeMap.erase(hcode);
			}
			m_existNode.erase(it);
		}
		boost::optional<const T2 &> findNode(T2 query) const
		{
			boost::optional<const T2 &> res;
			if (m_nodeMap.empty())
			{
				return res;
			}
			auto hashcode = std::hash<T2>{}(query);
			auto it = m_nodeMap.upper_bound(hashcode);
			if (it != m_nodeMap.end())
			{
				return it->second->identify;
			}
			return m_nodeMap.begin()->second->identify;
		}
	private:
		static const int	m_defaultVNum = 64;
		std::map<std::size_t, std::shared_ptr<Node>>	m_nodeMap;
		std::unordered_set<Node, NodeHash>			m_existNode;
	};
}

using namespace util;
#endif
