#include<vector>
#include<string>
#include<map>
using namespace std;

enum NodeType {
    Text_Node,
    Element_Data_Node
};

typedef map<string, string> AttrMap;

struct ElementData {
    string tag_name;
    AttrMap attributes;
		ElementData(const string& name, const AttrMap& attrs): tag_name(name), attributes(attrs) {}
};

struct Node{
	// data common to all nodes:
  vector<Node*> children;
	
	Node(){}
	Node(const vector<Node*>& nodeChildren): children(nodeChildren){}
};

struct TextNode: Node{
	string text; // data specific to each node type
	
	// constructor
	TextNode(const string& nodeText): text(nodeText) {}
};

struct ElementDataNode: Node{
	ElementData element; // data specific to each node type
	
	// constructor
	ElementDataNode(const string& name, const AttrMap& attrs, const vector<Node*>& nodeChildren): element(name, attrs), Node(nodeChildren) {}
};
