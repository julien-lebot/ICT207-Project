#include <Phoenix\GameEntity.hpp>
#include <vector>

using namespace Phoenix;

bool GameEntity::load()
{
	if( modelFile.size() == 0 )
	{
		return false;
	}
	else
	{
		Model model;
		SwmReader swmR;
		swmR.readFile ( modelFile, model );

		if (model.isStatic())
			m_pos = model.getPossition();
		
		std::vector<GLfloat> vertices = model.getVerticeVec();

		verticeBuffer = new HardwareBuffer(sizeof(&vertices[0]) * vertices.size() , HardwareBuffer::STATIC_DRAW, HardwareBuffer::VERTEX);
		verticeBuffer->upload(&vertices[0]);

		genIndicesBuffer(model.getFaceGroupVec());

	}
	return true;
}

void GameEntity::genIndicesBuffer ( const std::vector<FaceGroup>& fg )
{
	materialOffsets.push_back(0);
	std::vector<GLuint> indices;
	for(std::vector<FaceGroup>::const_iterator groupIter = fg.begin(); groupIter != fg.end(); groupIter++)
	{
		for(std::vector<FaceCollection>::const_iterator faceIter = (*groupIter).faces.begin(); faceIter != (*groupIter).faces.end(); faceIter++ )
		{
			indices.insert(indices.end(), (*faceIter).v.begin(), (*faceIter).v.end()); 
			materialOffsets.push_back(indices.size());
		}
	}
	
	indexBuffer = new HardwareBuffer(sizeof(&indices[0]) * indices.size(), HardwareBuffer::STATIC_DRAW, HardwareBuffer::INDEX);
	indexBuffer->upload(&indices[0]);
}

void GameEntity::display()
{
		glPushMatrix();
		glColor3f(1.0, 0.2, 0.2);
		glTranslatef(0.0, -5.0, 3.0f);
		
		verticeBuffer->bind();
		indexBuffer->bind();
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glIndexPointer(GL_UNSIGNED_INT, 0, 0);
		//glDrawArrays(GL_TRIANGLE_FAN,0,36);

		glDrawElements(GL_TRIANGLES, 17214, GL_UNSIGNED_INT, 0);
		
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
}