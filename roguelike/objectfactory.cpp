#include "objectfactory.h"

using namespace std;

vector<ObjectFactory *> ObjectFactory::factories;

char ObjectFactory::SYMB[] =
{
	// 0 ~ 4
	'|' ,
	')' ,
	'}' ,
	'[' ,
	']' ,
	// 5 ~ 9
	'(' ,
	'{' ,
	'\\',
	'=' ,
	'"' ,
	// 10 ~ 15
	'_' ,
	'~' ,
	'?' ,
	'!' ,
	'$' ,
	// 16 ~ 20
	'/' ,
	',' ,
	'-' ,
	'%' ,
	'&'
};

string ObjectFactory::TYPE[] =
{
	// 0 ~ 4
	"WEAPON"    ,
	"OFFHAND"   ,
	"RANGED"    ,
	"ARMOR"     ,
	"HELMET"    ,
	// 5 ~ 9
	"CLOAK"     ,
	"GLOVES"    ,
	"BOOTS"     ,
	"RING"      ,
	"AMULET"    ,
	// 10 ~ 15
	"LIGHT"     ,
	"SCROLL"    ,
	"BOOK"      ,
	"FLASK"     ,
	"GOLD"      ,
	// 16 ~ 20
	"AMMUNITION",
	"FOOD"      ,
	"WAND"      ,
	"CONTAINER" ,
	"STACK"
};

ObjectFactory::ObjectFactory()
{
	dhit = ddam = ddodge = ddef = dweight = dspeed = dattr = dval = NULL;
}

ObjectFactory::~ObjectFactory()
{
	if (dhit   ) delete dhit   ;
	if (ddam   ) delete ddam   ;
	if (ddodge ) delete ddodge ;
	if (ddef   ) delete ddef   ;
	if (dweight) delete dweight;
	if (dspeed ) delete dspeed ;
	if (dattr  ) delete dattr  ;
	if (dval   ) delete dval   ;
}

int ObjectFactory::deleteFactories()
{
	for (int i = 0; i < (int)factories.size(); i++) {
		delete factories[i];
	}
	factories.clear();

	return 0;
}

int ObjectFactory::initFields()
{
	dhit    = dice::parseDice(hit   );
	ddam    = dice::parseDice(dam   );
	ddodge  = dice::parseDice(dodge );
	ddef    = dice::parseDice(def   );
	dweight = dice::parseDice(weight);
	dspeed  = dice::parseDice(speed );
	dattr   = dice::parseDice(attr  );
	dval    = dice::parseDice(val   );

	itype  = Parser::parseType (type );
	icolor = Parser::parseColor(color);

	return 0;
}

int ObjectFactory::get_difficulty(){
  int d = 0;
  if(ddam)
    d += ddam->roll();

  if(dspeed)
    d += dspeed->roll();

  return d;
}

int ObjectFactory::load(const char *path)
{
	ifstream ifs(path);

	if (!ifs) {
		cerr << "Failed to open " << path << endl;
		return -1;
	}
	
	if (ifs.eof())
		return -1;

	// meta
	string line;
	getline(ifs, line);
	if (line=="RLG327 OBJECT DESCRIPTION 1") {
		while (!ifs.eof()) {
			ObjectFactory *of = next(ifs);

			if (of) {
				factories.push_back(of);
			} else {
				delete of;
			}
		}
		ifs.close();
		return 0;
	}
	ifs.close();
	return -1;
}

ObjectFactory *ObjectFactory::next(istream &is)
{
	ObjectFactory *of = new ObjectFactory;
	
	while (!is.eof()) {
		bool hasParsingError = false;

		string line;
		while (!is.eof()) {
			getline(is, line);
			if (line=="BEGIN OBJECT")
				break;
		}
		while (!is.eof() && !hasParsingError) {
			getline(is, line);
			string k;
			stringstream ss(line);
			ss >> k;
			if (k=="END") {
				break;
			} else if (k=="NAME") {
				if (!of->name.empty()) {
					hasParsingError = true;
				}
				Parser::trim(ss);
				getline(ss, of->name);
			} else if (k=="DESC") {
				if (!of->desc.empty()) {
					hasParsingError = true;
				}
				while(!is.eof()) {
					getline(is, line);
					if (line.c_str()[0]=='.') break;
					of->desc += line;
					if (is.peek()!='.')
						of->desc +='\n';
				}
			} else if (k=="TYPE") {
				if (!of->type.empty()) {
					hasParsingError = true;
				}
				Parser::trim(ss);
				getline(ss, of->type);

				if (0 > Parser::parseType(of->type)) {
					hasParsingError = true;
				}
			} else if (k=="COLOR") {
				if (!of->color.empty()) {
					hasParsingError = true;
				}
				Parser::trim(ss);
				getline(ss, of->color);

				if (0 > Parser::parseColor(of->color)) {
					hasParsingError = true;
				}
			} else if (k=="HIT") {	
				hasParsingError = Parser::parseDiceField(ss, of->hit);
			} else if (k=="DAM") {	
				hasParsingError = Parser::parseDiceField(ss, of->dam);
			} else if (k=="DODGE") {	
				hasParsingError = Parser::parseDiceField(ss, of->dodge);
			} else if (k=="DEF") {	
				hasParsingError = Parser::parseDiceField(ss, of->def);
			} else if (k=="WEIGHT") {	
				hasParsingError = Parser::parseDiceField(ss, of->weight);
			} else if (k=="SPEED") {	
				hasParsingError = Parser::parseDiceField(ss, of->speed);
			} else if (k=="ATTR") {	
				hasParsingError = Parser::parseDiceField(ss, of->attr);
			} else if (k=="VAL") {	
				hasParsingError = Parser::parseDiceField(ss, of->val);
			}
		}
		if (hasParsingError)
			continue;
		if (of->allFieldsFilled()) {
			of->initFields();
			return of;
		}
	}
	if (of) delete of;	
	return NULL;
}

bool ObjectFactory::allFieldsFilled()
{
	return !(
	name  .empty() || 
	desc  .empty() || 
	type  .empty() ||
	color .empty() || 
	hit   .empty() || 
	dam   .empty() ||
	dodge .empty() || 
	def   .empty() || 
	weight.empty() ||
	speed .empty() || 
	attr  .empty() || 
	val   .empty()
	);
}

ostream& operator<<(ostream& os, ObjectFactory &of)
{
	os << of.name   << endl;
	os << of.desc   << endl;
	os << of.type   << endl;
	os << of.color  << endl;
	os << of.hit    << endl;
	os << of.dam    << endl;
	os << of.dodge  << endl;
	os << of.def    << endl;
	os << of.weight << endl;
	os << of.speed  << endl;
	os << of.attr   << endl;
	os << of.val    << endl;
	return os;
}
