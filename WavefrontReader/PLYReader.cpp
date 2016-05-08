
#include"stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include "Mesh.h"
#include "PLYReader.h"
#include "supportClass.h"
#include <limits>
enum DataType
{
	_char,
	_uchar,
	_short,
	_ushort,
	_int,
	_uint,
	_float,
	_double,
	_list,
	Other
};

static char char_format[] = "%hhd";
static char uchar_format[] = "%hhu";
static char short_format[] = "%hd";
static char ushort_format[] = "%hu";
static char int_format[] = "%d";
static char uint_format[] = "%u";
static char float_format[] = "%f";
static char double_format[] = "%lf";

char* get_data_format(DataType data_type) {
	switch (data_type)
	{
	case _char:
		return char_format;
		break;
	case _uchar:
		return uchar_format;
		break;
	case _short:
		return short_format;
		break;
	case _ushort:
		return ushort_format;
		break;
	case _int:
		return int_format;
		break;
	case _uint:
		return uint_format;
		break;
	case _float:
		return float_format;
		break;
	case _double:
		return double_format;
		break;
	case _list:
		break;
	case Other:
		break;
	default:
		break;
	}
	return NULL;
}

struct Property {
	char		name[8];
	DataType	data_type;
	char		type_name[16];
	DataType	lst_type1;
	DataType	lst_type2;
};
#define MAX_ELEMENT_NAME 8

struct Element
{
	char		name[MAX_ELEMENT_NAME];
	int			length;
	std::vector<Property> property_list;
};

DataType str2type(const char* data_type) {
	DataType ret;
	if (strcmp(data_type, "char") == 0)
		ret = DataType::_char;
	else
		if (strcmp(data_type, "uchar") == 0)
			ret = DataType::_uchar;
		else
			if (strcmp(data_type, "short") == 0)
				ret = DataType::_short;
			else
				if (strcmp(data_type, "ushort") == 0)
					ret = DataType::_ushort;
				else
					if (strcmp(data_type, "int") == 0)
						ret = DataType::_int;
					else
						if (strcmp(data_type, "uint") == 0)
							ret = DataType::_uint;
						else
							if (strcmp(data_type, "float") == 0)
								ret = DataType::_float;
							else
								if (strcmp(data_type, "double") == 0)
									ret = DataType::_double;
	return ret;
}

union MyUnion
{
	char cvalue;
	unsigned char ucvalue;
	short svalue;
	unsigned  short usvalue;
	int ivalue;
	unsigned int uivalue;
	float fvalue;
	double dvalue;

};
typedef std::vector<MyUnion> VData;
#define MAX_NUM_ELEMENTS 10
#define MAX_FORMAT_NAME 10
#define MAX_FORMAT_VERSION 10

struct Ply_format {
	char format[MAX_FORMAT_NAME];
	char version[MAX_FORMAT_VERSION];
};

/*
 *	Read header of ply file
 *	Return elems a pointer to array of nelems
 */
void read_header(FILE* file, Element** elems, int* nelems, Ply_format* format) {

	Element* ret = new Element[MAX_NUM_ELEMENTS];
	*nelems = 0;
	if (file != NULL)
	{
		Element* current_editing = NULL;
		const int MAX_BUFF_LINE_HEADER = 128;
		while (1) {
			char lineHeader[MAX_BUFF_LINE_HEADER];
			fscanf(file, "%s", lineHeader);

			if (strcmp(lineHeader, "comment") == 0) {
				//Skip this line
				fscanf(file, "%*[^\n]\n", NULL);
			}

			if (strcmp(lineHeader, "format") == 0) {
				// Reading format
				fscanf(file, "%s %s", format->format, format->version);
			}

			if (strcmp(lineHeader, "element") == 0) {
				char element_type[MAX_ELEMENT_NAME];
				int  len = 0;
				fscanf(file, "%s %d", element_type, &len);
				// Create new element
				Element* new_elem = new Element();
				strcpy(new_elem->name, element_type);
				new_elem->length = len;
				elems[*nelems++] = new_elem;
				current_editing = new_elem;
				fprintf(stdout, "element %s %d\n", element_type, len);
			}

			if (strcmp(lineHeader, "property") == 0) {
				char data_type[12];
				char property_name[12];
				fscanf(file, "%s %s", data_type, property_name);
				// Create and update element property
				Property pt;
				Property* p = &pt;
				strcpy(p->name, property_name);
				if (strcmp(data_type, "char") == 0)
					p->data_type = DataType::_char;

				if (strcmp(data_type, "uchar") == 0)
					p->data_type = DataType::_uchar;

				if (strcmp(data_type, "short") == 0)
					p->data_type = DataType::_short;

				if (strcmp(data_type, "ushort") == 0)
					p->data_type = DataType::_ushort;

				if (strcmp(data_type, "int") == 0)
					p->data_type = DataType::_int;

				if (strcmp(data_type, "uint") == 0)
					p->data_type = DataType::_uint;

				if (strcmp(data_type, "float") == 0)
					p->data_type = DataType::_float;

				if (strcmp(data_type, "double") == 0)
					p->data_type = DataType::_double;
				if (strcmp(data_type, "list") == 0) {
					p->data_type = DataType::_list;
					char type1[8];
					strcpy(type1, property_name);
					char type2[8];
					char indices[16];
					fscanf(file, "%s %s", type2, indices);
					DataType t1 = str2type(type1);
					DataType t2 = str2type(type2);
					p->lst_type1 = t1;
					p->lst_type2 = t2;
				}
				// Adding to element
				current_editing->property_list.push_back(pt);
			}

			if (strcmp(lineHeader, "end_header") == 0) {
				break;
			}
		}
	}
	else
	{
		*elems = NULL;
		*nelems = 0;
		fprintf(stderr, "error: can not read this file.\n");
	}
}


Mesh* readFile(char* filename) {
	Element* vertex = new Element();
	strcpy(vertex->name, "vertex");
	vertex->length = 0;

	Element* face = new Element();
	strcpy(face->name, "face");
	face->length = 0;
	Mesh* mesh = new Mesh();
	static const char end_header[] = "end_header\n";
	FILE *file = fopen(filename, "r");

	if (file != NULL)
	{
		Element* current_editing = vertex;

		while (1) {
			char lineHeader[128];
			fscanf(file, "%s", lineHeader);

			if (strcmp(lineHeader, "comment") == 0) {
				// Nothing todo here
				fprintf(stdout, "%s", lineHeader);
				//Skip this line
				fscanf(file, "%*[^\n]\n", NULL);
			}

			if (strcmp(lineHeader, "format") == 0) {
				char format[6];
				char version[10];
				fscanf(file, "%s %s", format, version);
				fprintf(stdout, "format %s %s\n", format, version);
			}

			if (strcmp(lineHeader, "element") == 0) {
				char element_type[10];
				int  len = 0;
				fscanf(file, "%s %d", element_type, &len);
				// Update information
				if (strcmp(vertex->name, element_type) == 0) {
					vertex->length = len;
					current_editing = vertex;
				}
				else
					if (strcmp(face->name, element_type) == 0) {
						face->length = len;
						current_editing = face;
					}
					else {
						// Other element
						// TODO: Not implement...
					}
					fprintf(stdout, "element %s %d\n", element_type, len);
			}

			if (strcmp(lineHeader, "property") == 0) {
				char data_type[12];
				char property_name[12];
				fscanf(file, "%s %s", data_type, property_name);
				// Create and update element property
				Property pt;
				Property* p = &pt;
				strcpy(p->name, property_name);
				if (strcmp(data_type, "char") == 0)
					p->data_type = DataType::_char;
				else
					if (strcmp(data_type, "uchar") == 0)
						p->data_type = DataType::_uchar;
					else
						if (strcmp(data_type, "short") == 0)
							p->data_type = DataType::_short;
						else
							if (strcmp(data_type, "ushort") == 0)
								p->data_type = DataType::_ushort;
							else
								if (strcmp(data_type, "int") == 0)
									p->data_type = DataType::_int;
								else
									if (strcmp(data_type, "uint") == 0)
										p->data_type = DataType::_uint;
									else
										if (strcmp(data_type, "float") == 0)
											p->data_type = DataType::_float;
										else
											if (strcmp(data_type, "double") == 0)
												p->data_type = DataType::_double;
											else
												if (strcmp(data_type, "list") == 0) {
													p->data_type = DataType::_list;
													char type1[8];
													strcpy(type1, property_name);
													char type2[8];
													char indices[16];
													fscanf(file, "%s %s", type2, indices);
													DataType t1 = str2type(type1);
													DataType t2 = str2type(type2);
													p->lst_type1 = t1;
													p->lst_type2 = t2;
												}
				// Adding to element
				current_editing->property_list.push_back(pt);
			}

			if (strcmp(lineHeader, "end_header") == 0) {
				break;
			}
		}
		// Now read line by line
		// Vertex reading
		std::vector<std::string> ls_format;
		int count = vertex->property_list.size();
		std::map<std::string, int> name_map;
		int i = 0;
		for (auto const& value : vertex->property_list) {
			ls_format.push_back(std::string(get_data_format(value.data_type)));
			auto kv = std::pair<std::string, int>(std::string(value.name), i++);
			name_map.insert(kv);
		}

		mesh->numVerts = vertex->length;
		Point3* pt = new Point3[mesh->numVerts];
		mesh->pt = pt;
		mesh->numNorm = vertex->length;
		Vector3* norm = new Vector3[mesh->numNorm];
		mesh->norm = norm;

		for (int i = 0; i < vertex->length; i++)
		{
			// Reading vertext
			float x, y, z, nx, ny, nz;
			fscanf(file, "%f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz);
			pt[i].set(x, y, z);
			norm[i].set(nx, ny, nz);

			//fprintf(stdout, "%f %f %f %f %f %f\n", x, y, z, nx, ny, nz);
		}
		mesh->numFaces = face->length;
		mesh->face = new Face[mesh->numFaces];

		// Reading face
		for (int f = 0; f < face->length; f++) {
			//	printf("%d\n", f);
			int c = 0;
			
			unsigned char r, g, b, alpha;
			unsigned char color[4];
			fscanf(file, "%d", &c);
			int* vertextID = new int[c];
			float xmax = 0, ymax = 0, zmax = 0;
			float xmin = FLT_MAX;
		
			for (int j = 0; j < c; j++) {

				fscanf(file, "%d", &vertextID[j]);
			}
			fscanf(file, "%hhu %hhu %hhu %hhu", &color[0], &color[1], &color[2], &color[3]);
			mesh->face[f].nVerts = c;
			auto vid = new VertexID[c];
			mesh->face[f].vert = vid;
			_memccpy(mesh->face[f].color, &color, 0, sizeof(unsigned char) * 4);
			//fprintf(stdout, "%d %hhu %hhu %hhu %hhu \n", c, r, g, b, alpha);

			for (int i = 0; i < c; i++)
			{
				vid[i].vertIndex = vertextID[i];
				vid[i].normalIndex = vertextID[i];
			}
		}

		fclose(file);
	}
	else
	{
		perror(filename); /* why didn't the file open? */
	}
	//delete vertex;
	//delete face;
	return mesh;
}
