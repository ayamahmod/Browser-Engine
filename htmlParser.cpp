#include "htmlParser.h"
//#include <ctype>

// Return the current character, and advance self.pos to the next character.
char htmlParser::consume_char(){
	if(pos >= this->input.size())
		return '\0';
	char cur_char = this->input[this->pos];
	this->pos += 1;
	return cur_char;
}

// Consume characters until test returns false.
string htmlParser::consume_while(bool test(char)){
	string result = "";
	while (!this->eof() && test(this->next_char())){
		result+=(this->consume_char());
	}
	return result;
}

bool isWhiteSpace(char c){
	if(isspace(c))
		return true;
	return false;
}

// Consume and discard zero or more whitespace characters.
void htmlParser::consume_whitespace(){
	this->consume_while(isWhiteSpace);
}

bool isAlphanumeric(char c){
	if(isalnum(c))
		return true;
	return false;
}

// Parse a tag or attribute name.
string htmlParser::parse_tag_name(){
	this->consume_while(isAlphanumeric); 
}

// Parse a single node.
Node* htmlParser::parse_node(){
	if(this->next_char()=='<')
		return this->parse_element();
	
	return this->parse_text();
}

bool isnotLeftTag(char c){
	if(c != '<')	return true;
	return false;
}

// Parse a text node.
TextNode* htmlParser::parse_text(){
    return new TextNode(this->consume_while(isnotLeftTag));
}

// Parse a single element, including its open tag, contents, and closing tag.
ElementDataNode* htmlParser::parse_element(){
    // Opening tag.
    assert(this->consume_char() == '<');
    string tag_name = this->parse_tag_name();
    AttrMap attrs = this->parse_attributes();
    assert(this->consume_char() == '>');

    // Contents.
    vector<Node*> children = this->parse_nodes();

    // Closing tag.
    assert(this->consume_char() == '<');
    assert(this->consume_char() == '/');
    assert(this->parse_tag_name() == tag_name);
    assert(this->consume_char() == '>');

    return new ElementDataNode(tag_name, attrs, children);
}

// Parse a single name="value" pair.
pair<string, string> htmlParser::parse_attr(){
    string name = this->parse_tag_name();
    assert(this->consume_char() == '=');
    string value = this->parse_attr_value();
    return make_pair(name, value);
}

bool isnotOpenQuote(char c){
	if(c == '"' || c == '\'')
		return false;
	return true;
}

// Parse a quoted value.
string htmlParser::parse_attr_value(){
	char open_quote = this->consume_char();
	assert(open_quote == '"' || open_quote == '\'');
	string value = this->consume_while(isnotOpenQuote);
	assert(this->consume_char() == open_quote);
	return value;
}

// Parse a list of name="value" pairs, separated by whitespace.
AttrMap htmlParser::parse_attributes(){
	AttrMap attributes;
	while(1){
		this->consume_whitespace();
		if (this->next_char() == '>'){
				break;
		}
		attributes.insert(this->parse_attr());
	}
	return attributes;
}

// Parse a sequence of sibling nodes.
vector<Node*> htmlParser::parse_nodes(){
	vector<Node*> nodes;
	while(1){
		this->consume_whitespace();
		if (this->eof() || this->starts_with("</")){
				break;
		}
		nodes.push_back(this->parse_node());
	}
	return nodes;
}

// Parse an HTML document and return the root element.
Node* parse(const string& source){
	htmlParser* Parser = new htmlParser(0, source);
	vector<Node*> nodes = Parser->parse_nodes();

	// If the document contains a root element, just return it. Otherwise, create one.
	if (nodes.size() == 1){
			return nodes[0];
	} else {
			AttrMap attributes;
			return new ElementDataNode("html", attributes, nodes);
	}
}

int main()
{
	return 0;
}