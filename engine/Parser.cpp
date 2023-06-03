#include "headers/Parser.h"

using namespace std;
using namespace rapidxml;


Parser::~Parser()
{
    if (camera) delete camera;
    
    for (Light* l : lights)
        delete l;
    for (Group* g : groups)
    {
        if (g)
            delete g;
    }
    for (Shape* s : allShapes)
    {
        if (s) 
            delete s;
    }
    if (textureLoader) delete textureLoader;
    if (vboManager) delete vboManager;
}

/** 
 * @brief Parse the window section of the xml input file
 * @return int indicating if an error occured (0==ok, !=0 == not ok)
 */
int Parser::parseWindow (xml_node<>* windowNode)
{
	xml_attribute<>* width = windowNode->first_attribute("width");
	xml_attribute<>* height = windowNode->first_attribute("height");
	
	if (!width || !height) return 2;

	this->window = pair<float, float>(strtod(width->value(), NULL), strtod(height->value(),NULL));
	
	return 0;
}

int Parser::parseCamera (xml_node<>* cameraNode)
{
	xml_attribute<> *x, *y, *z;

	// Parsing camera position
	xml_node<>* node = cameraNode->first_node("position");

	if (node)
	{
		x = node->first_attribute("x");
		y = node->first_attribute("y");
		z = node->first_attribute("z");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera Position" << endl;
			return 2;
		}

		this->camera->setPosition(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
	}

	node = cameraNode->first_node("lookAt");

	if (node)
	{
		x = node->first_attribute("x");
		y = node->first_attribute("y");
		z = node->first_attribute("z");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera LookAt" << endl;
			return 2;
		}
		
		this->camera->setLookAt(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
	}
	
	node = cameraNode->first_node("up");

	if (node)
	{
		x = node->first_attribute("x");
		y = node->first_attribute("y");
		z = node->first_attribute("z");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera Up" << endl;
			return 2;
		}
			
		this->camera->setUp(new Point(strtod(x->value(), NULL), strtod(y->value(), NULL), strtod(z->value(), NULL)));
	}

	node = cameraNode->first_node("projection");

	if (node)
	{
		x = node->first_attribute("fov");
		y = node->first_attribute("near");
		z = node->first_attribute("far");

		if (!x || !y || !z)
		{
			cerr << "Invalid Camera Projection" << endl;
			return 2;
		}

		this->camera->setFov(strtod(x->value(), NULL));
		this->camera->setNear(strtod(y->value(), NULL));
		this->camera->setFar(strtod(z->value(), NULL));
	}

	return 0;
}

int Parser::parseLight(Light *light, rapidxml::xml_node<>* lightNode, int lightNum)
{
    if (!lightNode)
        return 0;
    LightType type;
    Point pos, dir;
    float cutoff = 0.0f;
    xml_attribute<>* typeAttr = lightNode->first_attribute("type");
    if (!typeAttr)
    {
        cerr << "Error: Light element should have a type attribute!" << endl;
        return 1;
    }
    string t = string(typeAttr->value());
    if (t == "point")
        type = point;
    else if (t == "directional")
        type = directional;
    else if (t == "spotlight" || t == "spot")
        type = spotlight;
    else
    {
        cerr << "Error: Light type should be 'point', 'directional' or 'spotlight'!" << endl;
        return 1;
    }

    if (type == point || type == spotlight)
    {
        xml_attribute<> *posX = lightNode->first_attribute("posX"),
                        *posY = lightNode->first_attribute("posY"),
                        *posZ = lightNode->first_attribute("posZ");
        if (!posX || !posY || !posZ)
        {
            
            posX = lightNode->first_attribute("posx");
            posY = lightNode->first_attribute("posy");
            posZ = lightNode->first_attribute("posz");

            if (!posX || !posY || !posZ)
            {
                cerr << "Error: light of type " << t << " should have posX, posY and poxZ attributes!" << endl;
                return 1;
            }
        }
        pos = Point(strtof(posX->value(), NULL),
                    strtof(posY->value(), NULL), 
                    strtof(posZ->value(), NULL));
    }

    if (type == directional || type == spotlight)
    {
        xml_attribute<> *dirX = lightNode->first_attribute("dirX"),
                        *dirY = lightNode->first_attribute("dirY"),
                        *dirZ = lightNode->first_attribute("dirZ");
        if (!dirX || !dirY || !dirZ)
        {
            dirX = lightNode->first_attribute("dirx");
            dirY = lightNode->first_attribute("diry");
            dirZ = lightNode->first_attribute("dirz");
            if (!dirX || !dirY || !dirZ)
            {
                cerr << "Error: light of type " << t << " should have dirX, dirY and dirZ attributes!" << endl;
                return 1;
            }
        }
        dir = Point(strtof(dirX->value(), NULL),
                    strtof(dirY->value(), NULL), 
                    strtof(dirZ->value(), NULL));
    }

    if (type == spotlight)
    {
        xml_attribute<> *cutoffAttr = lightNode->first_attribute("cutoff");

        if (!cutoffAttr)
        {
            cerr << "Error: light of type spotlight should have cutoff attribute!" << endl;
            return 1;
        }

        cutoff = strtof(cutoffAttr->value(), NULL);
    }
    
    static vector<GLenum> l = vector<GLenum>({GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7});
    *light = Light(type, pos, dir, cutoff, l[lightNum]);    
    return 0;
}

int Parser::parseLights(std::vector<Light*>* lights, rapidxml::xml_node<>* lightsNode)
{
    if (!lightsNode)
        return 0;
    int i=0;
    for (xml_node<>* lightNode = lightsNode->first_node("light") ; lightNode ; lightNode = lightNode->next_sibling("light"))
    {
        Light *light = (Light*)malloc(sizeof(Light));
        int error = this->parseLight(light, lightNode, i++);
        if (error)
        {
            delete light;
            return error;
        }
        lights->push_back(light);
    }

    return 0;
}

Transformation* parseTranslation(xml_node<>* transformationNode)
{
	TransfType type;
	xml_attribute<>* timeAttr = transformationNode->first_attribute("time");
	if (timeAttr)
	{
		type = TimeTranslate;
		float time = strtof(timeAttr->value(), NULL);
		xml_attribute<>* alignAttr = transformationNode->first_attribute("align");
		string alignStr = (alignAttr) ?string(alignAttr->value()) :"false";
		bool align = false;
		if (alignStr == "true")
			align = true;
		
		vector<Point> curvePoints;

		for (xml_node<>* point = transformationNode->first_node("point") ; point ; point = point->next_sibling("point"))
		{
			xml_attribute<> *x = point->first_attribute("x"), 
											*y = point->first_attribute("y"), 
											*z = point->first_attribute("z");

			if (!x || !y || !z)
			{
				cerr << "ERROR: Couldn't read point on translation definition!!" << endl;
				exit(1);
			}
			curvePoints.emplace_back(strtof(x->value(), NULL),
															 strtof(y->value(),NULL),
															 strtof(z->value(),NULL));
		}

		if (curvePoints.size() < 4)
		{
			cerr << "ERROR: Catmull-Rom curve should have at least 4 points!!" << endl;
			exit(1);
		}

		return new Curve(time, align, curvePoints);
	}
	float x = strtof(transformationNode->first_attribute("x")->value(), NULL);
	float y = strtof(transformationNode->first_attribute("y")->value(), NULL);
	float z = strtof(transformationNode->first_attribute("z")->value(), NULL);
	return new Transformation(Translate, x, y, z);
}

Transformation* parseRotation (xml_node<>* transformationNode)
{
	float x = strtof(transformationNode->first_attribute("x")->value(), NULL);
	float y = strtof(transformationNode->first_attribute("y")->value(), NULL);
	float z = strtof(transformationNode->first_attribute("z")->value(), NULL);
	xml_attribute<>* time = transformationNode->first_attribute("time");
	if (time) // caso seja uma rotacao temporal
		return new Transformation(x,y,z, strtof(time->value(), NULL));
	return new Transformation(Rotate,x,y,z, strtof(transformationNode->first_attribute("angle")->value(), NULL));
}

Transformation* parseTransformation(xml_node<>* transformationNode)
{
	if (transformationNode->type() == node_element)
	{
		TransfType type;
		if (strcmp(transformationNode->name(),"color")==0)
		{	
			type = Color;
			float r = strtof(transformationNode->first_attribute("r")->value(), NULL)/255.0f;
			float g = strtof(transformationNode->first_attribute("g")->value(), NULL)/255.0f;
			float b = strtof(transformationNode->first_attribute("b")->value(), NULL)/255.0f;
			return new Transformation(type, r, g, b, -1);
		}
		int aux = strcmp(transformationNode->name(),"scale");
		if (aux == 0)
		{
			type = Scale;
			float x = strtof(transformationNode->first_attribute("x")->value(), NULL);
			float y = strtof(transformationNode->first_attribute("y")->value(), NULL);
			float z = strtof(transformationNode->first_attribute("z")->value(), NULL);
			return new Transformation(type, x, y, z);
		}
		else if (aux < 0)
			return parseRotation(transformationNode);
		return parseTranslation(transformationNode);
	}
	return NULL;
}

int Parser::parseTransformations(vector<Transformation*>* transformations, xml_node<>* transformNode)
{
	if (!transformNode)
		return 0;
	xml_node<>* transformationNode;

	bool transl=false, rot=false, scale=false, color=false;

	for (transformationNode = transformNode->first_node() ; transformationNode ; transformationNode = transformationNode->next_sibling())
	{
		Transformation *transf = parseTransformation(transformationNode);
		int error = transf == NULL;
		if (!error)
		{
			bool *verify;
			switch (transf->getType())
			{
				case Translate: verify = &transl; break;
				case TimeTranslate: verify = &transl; break;
				case Rotate: verify = &rot; break;
				case TimeRotate: verify = &rot; break;
				case Color: verify = &color; break;
				default: verify = &scale; break;
			}
			if (!(*verify))
				*verify = true;
			else
			{
				cerr << "Error: Can't have more than 1 transformation of the same type!!" << endl;
				return 1;
			}
			transformations->push_back(transf);
		
		}
	}

	return 0;
}

int Parser::parseModel(Model* model, string filename)
{
	Shape* aux, *shape = model->getShape();
	if ((aux = this->getShapeFromShapes(shape->getName(), filename)))
	{
		if (aux->getFile() == "")
		{
			cerr << "Error: Only one Model can have the same \"name\" attribute!" << endl;
			return 1;
		}
		if (shape->getName() == aux->getName() && aux->getPoints().size() > 0)
		{
			for (Point p : aux->getPoints())
				shape->addPoint(p);
			return 0;
		}
	}

	string path = "../files/";
	path.append(filename);
	ifstream modelFile(path);

	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
	{
		cerr << "Error opening file \"" << filename << "\".\nMake sure the file exists...";
		return 1;
	}

	string line;
	if (!getline(modelFile, line))
	{
		cerr << "Error reading number of vertices of model!" << endl;
		return 1;
	}
	int numVertices = stof(line);
	
	for (int i=0 ; i<numVertices ; i++)
	{
		getline(modelFile, line);
		float x,y,z;
		if (sscanf(line.c_str(), "%f, %f, %f", &x, &y, &z) != 3)
		{
			cerr << "Error parsing vertex: model line number " << 2+i << " has wrong syntax...(\"" << line << "\")\n Make sure the file was generated correctly..";
			return 1;
		}

		shape->addPoint(Point(x,y,z));
	}

	for (int i=0 ; i<numVertices ; i++)
	{
		getline(modelFile, line);
		float x,y,z;
		if (sscanf(line.c_str(), "%f, %f, %f", &x, &y, &z) != 3)
		{
			cerr << "Error parsing normal: model line number " << 2+numVertices+i << " has wrong syntax...(\"" << line << "\")\n Make sure the file was generated correctly..";
			return 1;
		}

		shape->addNormal(Point(x,y,z));
	}

    for (int i=0 ; i<numVertices ; i++)
    {
		getline(modelFile, line);
		float x,y;
		if (sscanf(line.c_str(), "%f, %f", &x, &y) != 2)
		{
			cerr << "Error parsing texture coord: model line number " << 2+numVertices*2+i << " has wrong syntax...(\"" << line << "\")\n Make sure the file was generated correctly..";
			return 1;
		}

		shape->addTextCoords(Point2D(x,y));
    }

	return 0;
}

string Parser::parseTexture(xml_node<>* textureNode)
{
    if (!textureNode)
        return "";
    
    xml_attribute<>* file = textureNode->first_attribute("file");
    
    if (!file)
        return "";
    return string(file->value());
}

int Parser::parseColor(xml_node<>* colorNode, class::Color *color)
{
    if (!colorNode)
        return 0;

    xml_node<>  *child = colorNode->first_node("diffuse");
    if (child)
    {
        xml_attribute<> *r = child->first_attribute("R"),
                        *g = child->first_attribute("G"),
                        *b = child->first_attribute("B");
        if (!r || !g || !b)
        {
            cerr << "Error: Diffuse color should have R, G and B attributes!" << endl;
            return 1;
        }
        color->setDiffuse(Point(strtof(r->value(),NULL)/255.0f,
                                strtof(g->value(), NULL)/255.0f,
                                strtof(b->value(), NULL)/255.0f));
    }
    
    child = colorNode->first_node("ambient");
    if (child)
    {
        xml_attribute<> *r = child->first_attribute("R"),
                        *g = child->first_attribute("G"),
                        *b = child->first_attribute("B");
        if (!r || !g || !b)
        {
            cerr << "Error: Ambient color should have R, G and B attributes!" << endl;
            return 1;
        }
        color->setAmbient(Point(strtof(r->value(),NULL)/255.0f,
                                strtof(g->value(), NULL)/255.0f,
                                strtof(b->value(), NULL)/255.0f));
    }

    child = colorNode->first_node("specular");
    if (child)
    {
        xml_attribute<> *r = child->first_attribute("R"),
                        *g = child->first_attribute("G"),
                        *b = child->first_attribute("B");
        if (!r || !g || !b)
        {
            cerr << "Error: Specular color should have R, G and B attributes!" << endl;
            return 1;
        }
        float R = strtof(r->value(), NULL),
              G = strtof(g->value(), NULL),
              B = strtof(b->value(), NULL);
        color->setSpecular(Point(R, G, B)/255.0f);
    }

    child = colorNode->first_node("emissive");
    if (child)
    {
        xml_attribute<> *r = child->first_attribute("R"),
                        *g = child->first_attribute("G"),
                        *b = child->first_attribute("B");
        if (!r || !g || !b)
        {
            cerr << "Error: Emissive color should have R, G and B attributes!" << endl;
            return 1;
        }
        float R = strtof(r->value(), NULL),
              G = strtof(g->value(), NULL),
              B = strtof(b->value(), NULL);
        color->setEmissive(Point(R,G,B)/255.0f);
    }

    child = colorNode->first_node("shininess");
    if (child)
    {
        xml_attribute<>* value = child->first_attribute("value");
        if (!value)
        {
            cerr << "Error: Shininess should have value attribute!" << endl;
            return 1;
        }
        color->setShininess(strtof(value->value(), NULL));
    }

    return 0;
}

int Parser::parseModels(vector<Model*>* models, xml_node<>* modelsNode)
{
	if (!modelsNode)
		return 0;
	xml_node<>* model;
	for (model = modelsNode->first_node("model"); model; model = model->next_sibling("model"))
	{
			xml_attribute<>* file = model->first_attribute("file");
			if (!file)
			{
				cerr << "Error: No file attribute is specified no model" << endl;
				return 1;
			}
			string filename = string(file->value()), name;
			xml_attribute<>* nameAttr = model->first_attribute("name");
			if (nameAttr)
				name = nameAttr->value();
			else
				name = "";
			Shape* shape = new Shape(string(name), string(filename));

            string texture = this->parseTexture(model->first_node("texture"));
            class::Color color;
            
            if (this->parseColor(model->first_node("color"), &color))
            {
                cerr << "Error parsing color" << endl;
                return 1;
            }

            Model *model = new Model(shape, color, texture, textureLoader, vboManager); 

			int error = this->parseModel(model, filename);
			if (error)
			{
				cerr << "Error parsing model" << endl;
				return error;
			}
			models->push_back(model);
			this->allShapes.push_back(shape);;
	}
	return 0;
}

int Parser::parseGroup(Group* group, xml_node<>* groupNode)
{
	xml_node<>* transfNode = groupNode->first_node("transform");
	vector<Transformation*> transformations;
	int error = this->parseTransformations(&transformations, transfNode);

	if (error) // if transmodeltions is NULL that means that an exception occured during the parse
	{
		cerr << "Error parsing Transformations" << endl;
		return error;
	}
	group->addTransformations(transformations);

	xml_node<>* modelsNode = groupNode->first_node("models");
	vector<Model*> models;
	error = this->parseModels(&models, modelsNode);
	if (error)
	{
		cerr << "Error parsing Models!!" << endl;
		return error;
	}
	group->addModels(models);
    group->generateVBOs();

	vector<Group*> groups;
	error = this->parseGroups(&groups, groupNode);
	if (error)
	{
		cerr << "Error Parsing Groups" << endl;
		return error;
	}
	group->addGroups(groups);

	return 0;
}

int Parser::parseGroups(vector<Group*>* groups, xml_node<>* node)
{
	if (!node)
		return 0;
	xml_node<>* groupNode;
	for (groupNode = node->first_node("group") ; groupNode ; groupNode = groupNode->next_sibling("group"))
	{
		Group *group = new Group();
		int error = this->parseGroup(group, groupNode);
		if (error)
			return error;
		groups->push_back(group);
	}

	return 0;
}

int Parser::parseXML(char *filePath)
{
	xml_document<> document;
	xml_node<> * root_node = NULL;

	string path = "../files/";
	path.append(filePath);
	ifstream file(path);
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	document.parse<0>(&buffer[0]);

	root_node = document.first_node("world");

	if (!root_node)
	{
		cerr << "Error reading file" << endl;
		return 1;
	}

	int rv = this->parseWindow(root_node->first_node("window"));
	rv = rv | this->parseCamera(root_node->first_node("camera"));
    
    vector<Light*> lights;
    rv = rv | this->parseLights(&lights, root_node->first_node("lights"));
    this->lights = lights;

	vector<Group*> groups;
	rv = rv | this->parseGroups(&groups, root_node);
	this->groups = groups;

	file.close();

	if (rv)
		return rv;

	return 0;
}

Shape* Parser::getShapeFromShapes(std::string name, std::string file)
{
	if (name == "")
	{
		for (Shape* s : allShapes)
			if (s->getFile() == file)
				return s;
		return new Shape(name, file);
	}
	Shape* found = NULL;
	bool sameName = false;
	for (Shape* s : allShapes)
	{
		if (s->getName() == name)
			return new Shape();
		else if (s->getFile()==file)
			found = s;
	}
	return found;
}

pair<float, float> Parser::getWindow()
{
	return this->window;
}

Camera* Parser::getCamera()
{
	return this->camera;
}

vector<Light*> Parser::getLights()
{
    return lights;
}

vector<Group*> Parser::getGroups()
{
	return groups;
}

vector<Shape*> Parser::getShapes()
{
	return allShapes;
}
