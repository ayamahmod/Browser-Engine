#include <string>
#include "DOM.h"
using namespace std;

class htmlParser{
	
    unsigned int pos; 
    const string input;
		
	public:
	
		htmlParser(unsigned int position, const string& source):pos(position), input(source){}
		
		// Read the current character without consuming it.
    char next_char(){
        return this->input[this->pos];
    }

    // Do the next characters start with the given string?
    bool starts_with(string s){
        return (this->input.find(s) == this->pos);
    }

    // Return true if all input is consumed.
    bool eof(){
        return (this->pos >= this->input.size());
    }
	
		// Return the current character, and advance self.pos to the next character.
		char consume_char();
		
		// Consume characters until test returns false.
		string consume_while(bool test(char));
		
		// Consume and discard zero or more whitespace characters.
		void consume_whitespace();
		
		// Parse a tag or attribute name.
		string parse_tag_name();
		
		// Parse a single node.
		Node* parse_node();
		
		// Parse a text node.
		TextNode* parse_text();

		// Parse a single element, including its open tag, contents, and closing tag.
		ElementDataNode* parse_element();
		
		// Parse a single name="value" pair.
		pair<string, string> parse_attr();
		
		// Parse a quoted value.
		string parse_attr_value();
		
		// Parse a list of name="value" pairs, separated by whitespace.
		AttrMap parse_attributes();
		
		// Parse a sequence of sibling nodes.
		vector<Node*> parse_nodes();	
};