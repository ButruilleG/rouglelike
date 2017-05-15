#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "dice.h"
#include "parser.h"
#include "core.h"

using namespace std;

class ObjectFactory
{
	private:
		dice *dhit;
		//dice *ddam;
		dice *ddodge;
		dice *ddef;
		dice *dweight;
		//dice *dspeed;
		dice *dattr;
		dice *dval;

		int icolor;
		int itype; // type index

	public:
		/* need to be available for rolling */
		dice *ddam;
		dice *dspeed;
		
		string name;
		string desc;
		string type;
		string color;
		string hit;
		string dam;
		string dodge;
		string def;
		string weight;
		string speed;
		string attr;
		string val;

		int get_difficulty();

		ObjectFactory();
		~ObjectFactory();

		static char   SYMB[];
		static string TYPE[];

		static vector<ObjectFactory *> factories;

		bool allFieldsFilled();

		static int deleteFactories();
	
		// convert all string fields to object fields
		int initFields();

		static int load(const char *path);

		static ObjectFactory *next(istream &is);
};

ostream &operator<<(ostream &os, ObjectFactory &);

extern dice* barehanded;

#endif
