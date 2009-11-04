/*
*	Material.hpp
*
*	Material class. Stores a material
*
*	Created by Terje Loe
*/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <Phoenix/Color.hpp>
#include <Phoenix/Texture.hpp>

namespace Phoenix
{
	/* @brief material class
	*
	* The class is used to store materials
	*/
	class Material
	{
	public:
		/// Default constructor
		Material(){}

		/// Copy constructor
		Material (const Phoenix::Material& rhs)
			: materialName(rhs.materialName), materialAmbient(rhs.materialAmbient), materialDiffuse(rhs.materialDiffuse), materialSpecular(rhs.materialSpecular), transparency(rhs.transparency), shininess(rhs.shininess), illumination(rhs.illumination), ambientMap(rhs.ambientMap), diffuseMap(rhs.diffuseMap), specularMap(rhs.specularMap), bumpMap(rhs.bumpMap)
		{}

		/// Assignment operator
		Phoenix::Material& operator= ( const Phoenix::Material& rhs)
		{
			materialName = rhs.materialName;
			materialAmbient = rhs.materialAmbient;
			materialDiffuse = rhs.materialDiffuse;
			materialSpecular = rhs.materialSpecular;
			transparency = rhs.transparency;
			shininess = rhs.shininess;
			illumination = rhs.illumination;
			ambientMap = rhs.ambientMap;
			diffuseMap = rhs.diffuseMap;
			specularMap = rhs.specularMap;
			bumpMap = rhs.bumpMap;		
			return *this;
		}

		/// Destructor
		~Material(){}

		/* sets material name
		* @param name string
		*/
		void setName ( std::string name ) { materialName = name; }

		/* sets the material ambient color
		* @param Color object
		*/
		void setMaterialAmbient ( Phoenix::Color col ) { materialAmbient = col; }

		/* sets the material diffuse color
		* @param Color object
		*/
		void setMaterialDiffuse ( Phoenix::Color col ) { materialDiffuse = col; }

		/* sets the materal specular color
		* @param Color object
		*/		
		void setMaterialSpecular ( Phoenix::Color col ) { materialSpecular = col; }

		/* sets the material transparency
		* @param transparency float value
		*/
		void setTransparency ( float tr ) { transparency = tr; }	

		/* sets the material shininess
		* @param shininess float value
		*/
		void setShininess ( float s ) { shininess = s;}

		/* sets the material illumination
		* @param illumination int value
		*/
		void setIllum ( int il ) { illumination = il; }

		/* sets the ambient map filename
		* @param filename string
		*/
		void setAmbientMap ( std::string map ) { ambientMap = map; }

		/* sets the diffuse map filename
		* @param filename string
		*/
		void setDiffuseMap ( std::string map ) { diffuseMap = map; }

		/* sets the specular map filename
		* @param filename string
		*/
		void setSpecularMap ( std::string map ) { specularMap = map; }

		/* sets the bump map filename
		* @param filename string
		*/
		void setBumpMap ( std::string map ) { bumpMap = map; }

		/* gets the material name
		* @return const name string
		*/
		const std::string getName () const { return materialName; }

		/* gets the material ambient color
		* @return const color object
		*/		
		const Phoenix::Color getMaterialAmbient () const { return materialAmbient; }

		/* gets the material diffuse color
		* @return const color object
		*/
		const Phoenix::Color getMaterialDiffuse () const { return materialDiffuse; }

		/* gets the material specular color
		* @return const color object
		*/
		const Phoenix::Color getMaterialSpecular () const { return materialSpecular; }

		/* gets the material transparency
		* @return const transparency float
		*/
		const float getTransparency () const  { return transparency; }	

		/* gets the material shininess
		* @return const shininess float
		*/
		const float getShininess () const { return shininess;}

		/* gets the material illumination
		* @return const int illumination int
		*/
		const int getIllum () const { return illumination; }

		/* gets the ambient map filename
		* @return filename string
		*/
		const std::string getAmbientMap () const { return ambientMap; }

		/* gets the diffuse map filename
		* @return filename string
		*/
		const std::string getDiffuseMap () const { return diffuseMap; }

		/* gets the specular map filename
		* @return filename string
		*/
		const std::string getSpecularMap () const { return specularMap; }

		/* gets the bump map filename
		* @return filename string
		*/
		const std::string getBumpMap () const { return bumpMap; }

		const ResourcePtr & getDiffuseTexture() const {return diffuseTexture;}

		void loadTextures()
		{
			if(diffuseMap == "")
				diffuseMap = "default.tga";
			
			diffuseTexture = TexturePtr(new Texture(NULL, diffuseMap, 0, TEXTURE_2D));
			diffuseTexture->setFilePath("maps/" + diffuseMap);
			diffuseTexture->prepare();
			diffuseTexture->load();
		}
	private:
		/// Name of the material
		std::string materialName;

		/// Material ambient color
		Phoenix::Color materialAmbient;

		/// Material diffuse color
		Phoenix::Color materialDiffuse;

		/// Material specular color
		Phoenix::Color materialSpecular;

		/// Material transparency
		float transparency;

		/// Material shininess
		float shininess;

		/// Material illumination
		int illumination;

		/// Ambient map filename
		std::string ambientMap;

		/// Diffuse map filename
		std::string diffuseMap;

		/// specular map filename
		std::string specularMap;

		/// bump map filename
		std::string bumpMap;

		/// diffuse texture
		ResourcePtr diffuseTexture;
	};
}

#endif