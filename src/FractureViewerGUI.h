#ifndef FractureViewerGUI_H
#define FractureViewerGUI_H

#include "bgui.h"
#include "Window.h"

class FractureViewer;

class FractureViewerGUI
{
public:
	FractureViewerGUI(FractureViewer* scene) : m_scene(scene) {};
	~FractureViewerGUI();

	bool Init(Window* window);
	void Load();
	void Render();
	void ProcessMouseLeftButtonDown(int x, int y);
	void ProcessMouseRightButtonDown(int x, int y);
	void ProcessMouseCenterButtonDown(int x, int y);
	void ProcessMouseLeftButtonUp(int x, int y);
	void ProcessMouseRightButtonUp(int x, int y);
	void ProcessMouseCenterButtonUp(int x, int y);
	void ProcessMouseMotion(int x, int y);
	bool IsPointerInside(int x, int y);
	void UpdateGUI();
	void SetVisible(bool flag);
	bool GetVisible();

	//Callbacks (modificar dependendiendo de la GUI)
	void OnButtonDown(int id);
	void OnClick(int id);
	void OnMenu(int id);
	void OnSelectModel(int sel);
	void OnSelectShader(int sel);
	void OnSelectCheckBox(int sel);
	void OnSelectDiffuseTexture(int sel);
	void OnSelectSpecularTexture(int sel);
	void OnSelectAmbientTexture(int sel);
	void OnSelectEmissiveTexture(int sel);
	void OnSelectNormalTexture(int sel);
	void OnSelectAlphaTexture(int sel);
	void OnSelectCameraType(int sel);
	void OnSelectBoundingBox(int sel);
	void OnSelectCubeMapFaceNegX(int sel);
	void OnSelectCubeMapFaceNegY(int sel);
	void OnSelectCubeMapFaceNegZ(int sel);
	void OnSelectCubeMapFacePosX(int sel);
	void OnSelectCubeMapFacePosY(int sel);
	void OnSelectCubeMapFacePosZ(int sel);

private:
	std::string m_meshSelected = "";
	std::string m_shaderSelected = "";
	std::string m_diffuseTextureSelected = "";
	std::string m_specularTextureSelected = "";
	std::string m_ambientTextureSelected = "";
	std::string m_emissiveTextureSelected = "";
	std::string m_normalTextureSelected = "";
	std::string m_alphaTextureSelected = "";
	std::string m_negX = "";
	std::string m_negY = "";
	std::string m_negZ = "";
	std::string m_posX = "";
	std::string m_posY = "";
	std::string m_posZ = "";

	// Variables del GUI
	Window* m_window;
	begui::Window m_wnd1;
	begui::TabContainer m_tabs;
	begui::Container mainContainer;
	bool m_bIsVisible = false;

	//Escena
	begui::Label m_lblScene;
	begui::ListBox m_lstScene;
	begui::Group m_groupTransform;
	begui::Label m_lblPosX;
	begui::Label m_lblPosY;
	begui::Label m_lblPosZ;
	begui::Label m_lblRotX;
	begui::Label m_lblRotY;
	begui::Label m_lblRotZ;
	begui::Slider m_sldPosX;
	begui::Slider m_sldPosY;
	begui::Slider m_sldPosZ;
	begui::Slider m_sldRotX;
	begui::Slider m_sldRotY;
	begui::Slider m_sldRotZ;

	begui::Group m_groupMesh;
	begui::CheckBox m_ckbCulling;
	begui::Label m_lblMeshInfo;
	begui::Label m_lblMeshVerts;
	begui::Label m_lblNumVerts;
	begui::Label m_lblMeshFaces;
	begui::Label m_lblNumFaces;

	begui::Group m_groupCamera;
	begui::Label m_lblCameraType;
	begui::ListBox m_lstCameraType;

	//Render
	begui::Button m_btnRender; 
	begui::Group m_groupScene;
	begui::CheckBox m_ckbWireframe;
	begui::Slider m_sldWireframe;
	begui::CheckBox m_ckbGrid;

	begui::Group m_groupSkyLight;
	begui::CheckBox m_ckbSkyLight;
	begui::Label m_lblSkyLightIntensity;
	begui::Slider m_sldSkyLightIntensity;
	begui::CheckBox m_ckbSkyReflections;

	begui::Group m_groupBoundingBox;
	begui::CheckBox m_ckbBoundingBox;
	begui::Label m_lblBoundingBox;
	begui::ListBox m_lstBoundingBox;

	//Animacion
	begui::Group m_groupSkeleton;
	begui::CheckBox m_ckbJoints;
	begui::CheckBox m_ckbSkinning;

	//Material
	begui::Label m_lblSelShader;
	begui::ComboBox m_cbxShaders;
	begui::Group m_groupTextures;
	begui::Label m_lblDiffuse;
	begui::Button m_btnDiffuse;
	begui::Button m_btnDelDiffuse;
	begui::TextBox m_txtDiffuse;
	begui::Label m_lblSpecular;
	begui::Button m_btnSpecular;
	begui::Button m_btnDelSpecular;
	begui::TextBox m_txtSpecular;
	begui::Label m_lblAmbient;
	begui::Button m_btnAmbient;
	begui::Button m_btnDelAmbient;
	begui::TextBox m_txtAmbient;
	begui::Label m_lblEmissive;
	begui::Button m_btnEmissive;
	begui::Button m_btnDelEmissive;
	begui::TextBox m_txtEmissive;
	begui::Label m_lblNormal;
	begui::Button m_btnNormal;
	begui::Button m_btnDelNormal;
	begui::TextBox m_txtNormal;
	begui::Label m_lblAlpha;
	begui::Button m_btnAlpha;
	begui::Button m_btnDelAlpha;
	begui::TextBox m_txtAlpha;

	begui::Group m_groupMatProperties;
	begui::Label m_lblMatColor;
	begui::Label m_lblColorR;
	begui::Label m_lblColorG;
	begui::Label m_lblColorB;
	begui::Slider m_sldColorR;
	begui::Slider m_sldColorG;
	begui::Slider m_sldColorB;
	begui::Label m_lblSpecIntensity;
	begui::Slider m_sldSpecIntensity;
	begui::Label m_lblSpecPower;
	begui::Slider m_sldSpecPower;
	begui::Label m_lblEmissionContribution;
	begui::Slider m_sldEmissionContribution;
	begui::Label m_lblAmbientContribution;
	begui::Slider m_sldAmbientContribution;
	begui::Label m_lblDiffuseContribution;
	begui::Slider m_sldDiffuseContribution;
	begui::Label m_lblSpecularContribution;
	begui::Slider m_sldSpecularContribution;
	begui::Label m_lblTiling;
	begui::Slider m_sldTiling;
	begui::Button m_btnResTiling;

	//Luces
	begui::Group m_groupAmbientLight;
	begui::CheckBox m_ckbAmbientLightActive;
	begui::Label m_lblAmbientLightColor;
	begui::Label m_lblAmbientLightColorR;
	begui::Label m_lblAmbientLightColorG;
	begui::Label m_lblAmbientLightColorB;
	begui::Slider m_sldAmbientLightColorR;
	begui::Slider m_sldAmbientLightColorG;
	begui::Slider m_sldAmbientLightColorB;

	begui::Group m_groupDirectionalLight;
	begui::CheckBox m_ckbDirectionalLightActive;
	begui::Label m_lblDirectionalLightDiffColor;
	begui::Label m_lblDirectionalLightDiffColorR;
	begui::Label m_lblDirectionalLightDiffColorG;
	begui::Label m_lblDirectionalLightDiffColorB;
	begui::Slider m_sldDirectionalLightDiffColorR;
	begui::Slider m_sldDirectionalLightDiffColorG;
	begui::Slider m_sldDirectionalLightDiffColorB;
	begui::Label m_lblDirectionalLightSpecColor;
	begui::Label m_lblDirectionalLightSpecColorR;
	begui::Label m_lblDirectionalLightSpecColorG;
	begui::Label m_lblDirectionalLightSpecColorB;
	begui::Slider m_sldDirectionalLightSpecColorR;
	begui::Slider m_sldDirectionalLightSpecColorG;
	begui::Slider m_sldDirectionalLightSpecColorB;
	begui::Label m_lblDirectionalLightIntensity;
	begui::Slider m_sldDirectionalLightIntensity;
	begui::Label m_lblDirectionalLightDirection;
	begui::Label m_lblDirectionalLightDirectionX;
	begui::Slider m_sldDirectionalLightDirectionX;
	begui::Label m_lblDirectionalLightDirectionY;
	begui::Slider m_sldDirectionalLightDirectionY;
	begui::Label m_lblDirectionalLightDirectionZ;
	begui::Slider m_sldDirectionalLightDirectionZ;

	//Menu
	begui::Dialog m_dlg1;
	begui::ListBox m_lst1;
	begui::Button m_dlgBtn1;

	//Cubemaps
	begui::Dialog m_dlg2;
	begui::Button m_dlgBtn2;
	begui::Label m_lblNegXCbp;
	begui::Label m_lblNegYCbp;
	begui::Label m_lblNegZCbp;
	begui::Label m_lblPosXCbp;
	begui::Label m_lblPosYCbp;
	begui::Label m_lblPosZCbp;
	begui::TextBox m_txtNegXCbp;
	begui::TextBox m_txtNegYCbp;
	begui::TextBox m_txtNegZCbp;
	begui::TextBox m_txtPosXCbp;
	begui::TextBox m_txtPosYCbp;
	begui::TextBox m_txtPosZCbp;
	begui::Button m_btnNegXCbp;
	begui::Button m_btnNegYCbp;
	begui::Button m_btnNegZCbp;
	begui::Button m_btnPosXCbp;
	begui::Button m_btnPosYCbp;
	begui::Button m_btnPosZCbp;
	begui::Button m_btnDelNegXCbp;
	begui::Button m_btnDelNegYCbp;
	begui::Button m_btnDelNegZCbp;
	begui::Button m_btnDelPosXCbp;
	begui::Button m_btnDelPosYCbp;
	begui::Button m_btnDelPosZCbp;

	FractureViewer* m_scene;
};

#endif
