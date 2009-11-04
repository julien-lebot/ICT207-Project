#include <SpaceWings/GameEntity.hpp>
#include <vector>

using namespace Phoenix;

void GameEntity::load(SwmReader & swmR)
{
	Model model;
	swmR.readFile ( modelFile, model );
	if(model.isEmpty())
	{
		loaded = false;
		return;
	}

	mdlVertices = model.getVerticeVec();
	mdlNormals = model.getVNormalVec();
	mdlTexCoords = model.getVTextureVec();

	std::size_t vertexSize = sizeof(&mdlVertices[0]) * mdlVertices.size();
	std::size_t normalSize = sizeof(&mdlNormals[0]) * mdlNormals.size();
	std::size_t texCoordSize = sizeof(&mdlTexCoords[0]) * mdlTexCoords.size();
	std::size_t totalSize = vertexSize + normalSize + texCoordSize;

	HardwareBufferPtr vertexBuffer = HardwareBufferPtr(new HardwareBuffer(totalSize, HardwareBuffer::STATIC_DRAW, HardwareBuffer::VERTEX));
	vertexBuffer->upload(0, vertexSize, &mdlVertices[0]);
	vertexBuffer->upload(vertexSize, normalSize, &mdlNormals[0]);
	vertexBuffer->upload(vertexSize + normalSize, texCoordSize, &mdlTexCoords[0]);

	m_renderOp.indexed = false;

	m_renderOp.vertexData = new VertexData();
	m_renderOp.vertexData->count = model.getVerticeCount();
	m_renderOp.vertexData->start = 0;
	m_renderOp.vertexData->bufferElementGroup.addElement(0,0,POSITION,FLOAT3);
	m_renderOp.vertexData->bufferElementGroup.addElement(0,vertexSize,NORMAL,FLOAT3);
	m_renderOp.vertexData->bufferElementGroup.addElement(0,vertexSize + normalSize,TEXTURE_COORDINATES,FLOAT3);
	m_renderOp.vertexData->bufferBinding[0] = vertexBuffer;
	m_renderOp.primitiveType = RenderOperation::TRIANGLES;

	m_renderOp.mtlOffsets = model.getMtlOffsets();
	m_renderOp.mtlOffsetNames = model.getMtlOffsetNames ();
	m_renderOp.mtlList = model.getMtlListVec ();
	
	for (std::vector<Phoenix::Material>::iterator iter = m_renderOp.mtlList.begin(); iter != m_renderOp.mtlList.end(); ++iter)
		(*iter).loadTextures();

	loaded = true;
}