#include "Shape.h"
#include "Transformation.h"

#include <vector>

class Group
{
	private:
		std::vector<Transformation> transformations;
		std::vector<Shape> models;
		std::vector<Group> groups;

	public:
		Group();
		Group(Group& group);
		~Group();

		void addTransformation(Transformation t);
		void addModel(Shape s);
		void addGroup(Group g);

		std::vector<Transformation> getTransformations();
		std::vector<Shape> getModels();
		std::vector<Group> getGroups();
};
