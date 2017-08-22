#include "FractureViewerGUI.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "FractureViewer.h"

std::vector<std::string> GetAllFilesnamesWithinFolder(std::string folder, std::string type)
{
	std::vector<std::string> names;
	char search_path[200];
	sprintf(search_path, "%s*.%s", folder.c_str(), type.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// leemos todas los archivos en la carpeta actual
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it)
	{
		std::cout << *it << std::endl;
	}

	return names;
}

int prevx = 0;
int prevy = 0;

bool FractureViewerGUI::Init(Window* window)
{
	m_window = window;

	//Inicializamos los recursos de la interfaz grafica
	// nos guardamos el directorio de Windows
	char win_dirc[MAX_PATH + 1];
	GetWindowsDirectory(win_dirc, MAX_PATH);
	char win_dir[MAX_PATH + 1] = "c:\\windows";

	// inicializamos los subsistemas de las fuentes y recursos
	if (!begui::FontManager::initialize())
	{
		printf("ERROR: No se pudo inicializar el subsistema de fuentes\n");
		return false;
	}
	if (!begui::FontManager::setFont(strcat(win_dir, "\\Fonts\\tahoma.ttf"), 11)) 
	{
		printf("ERROR: No se pudo cargar la fuente por defecto\n");
		return false;
	}

	// cargamos los recursos de la interfaz
	begui::ResourceManager::inst()->setResourceDir("./res/gui/");
	begui::ResourceManager::inst()->loadResources();

	return true;
}

void FractureViewerGUI::Load()
{
	//glClearColor(255.0f, 255.0f, 255.0f, 255.0f);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	m_wnd1.create(0, 0, 300, 750, "Fracture Viewer");
	m_wnd1.enableClose(false);
	m_wnd1.setMovable(false);

	begui::Menu& menu = *m_wnd1.createMainMenu();

	begui::Menu* pFileMenu = menu.addMenuItem("Archivo", 1, makeFunctor(*this, &FractureViewerGUI::OnMenu));

	pFileMenu->addMenuItem("Importar modelo", 101, makeFunctor(*this, &FractureViewerGUI::OnMenu));
	pFileMenu->addMenuItem("Cargar cubemap", 102, makeFunctor(*this, &FractureViewerGUI::OnMenu));
	pFileMenu->addSeparator();
	pFileMenu->addMenuItem("Salir", 106, makeFunctor(*this, &FractureViewerGUI::OnMenu));

	begui::Menu *pMenu2 = menu.addMenuItem("Vista", 2, makeFunctor(*this, &FractureViewerGUI::OnMenu));
	pMenu2->addMenuItem("Pantalla completa", 201, makeFunctor(*this, &FractureViewerGUI::OnMenu));

	m_tabs.create(8, 25, 280, 660);
	m_tabs.addTab("Escena");
	m_tabs.addTab("Render");
	m_tabs.addTab("Anim");
	m_tabs.addTab("Mat");
	m_tabs.addTab("Luces");
	m_wnd1.addComponent(&m_tabs);
	
	// Escena
	m_lblScene.create(125, 10, "ESCENA");
	m_lstScene.create(15, 30, 250, 200, begui::ListBox::MULTI_SELECT_SINGLECLICK);
	m_lstScene.addItem("Malla");
	m_lstScene.setAutoHeight(false);

	m_groupTransform.create(15, 240, 250, 185, "Transformacion");
	m_lblPosX.create(15, 30, "Posicion X: ");
	m_sldPosX.create(80, 30, 125);
	m_sldPosX.setBounds(-100, 100, 0);
	m_sldPosX.bindValue(&m_scene->tX);
	m_lblPosY.create(15, 55, "Posicion Y: ");
	m_sldPosY.create(80, 55, 125);
	m_sldPosY.setBounds(-100, 100, 0);
	m_sldPosY.bindValue(&m_scene->tY);
	m_lblPosZ.create(15, 80, "Posicion Z: ");
	m_sldPosZ.create(80, 80, 125);
	m_sldPosZ.setBounds(-100, 100, 0);
	m_sldPosZ.bindValue(&m_scene->tZ);
	m_lblRotX.create(15, 105, "Rotacion X: ");
	m_sldRotX.create(80, 105, 125);
	m_sldRotX.setBounds(0, 360, 0);
	m_lblRotY.create(15, 130, "Rotacion Y: ");
	m_sldRotY.create(80, 130, 125);
	m_sldRotY.setBounds(0, 360, 0);
	m_lblRotZ.create(15, 155, "Rotacion Z: ");
	m_sldRotZ.create(80, 155, 125);
	m_sldRotZ.setBounds(0, 360, 0);

	m_groupMesh.create(15, 445, 250, 100, "Malla");
	m_ckbCulling.create(20, 25, "Cull Back Faces", 302);
	m_ckbCulling.setState(true);
	m_ckbCulling.handleOnClick(makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_lblMeshInfo.create(100, 50, "Info malla");
	m_lblMeshVerts.create(20, 70, "Vertices malla: ");
	m_lblNumVerts.create(100, 70, "");
	if(m_scene->GetMesh()!= nullptr) m_lblNumVerts.setText(std::to_string(m_scene->GetMesh()->GetMesh()->GetNumVertices()));
	else m_lblNumVerts.setText("0");
	m_lblMeshFaces.create(150, 70, "Caras malla: ");
	m_lblNumFaces.create(220, 70, "");
	if (m_scene->GetMesh() != nullptr) m_lblNumFaces.setText(std::to_string(m_scene->GetMesh()->GetMesh()->GetNumFaces()));
	else m_lblNumFaces.setText("0");

	m_groupCamera.create(15, 565, 250, 80, "Camara");
	m_lblCameraType.create(90, 20, "Tipo Camara");
	m_lstCameraType.create(20, 35, 215, 200, begui::ListBox::SINGLE_SELECT);
	m_lstCameraType.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectCameraType));
	m_lstCameraType.addItem("Objetivo");
	m_lstCameraType.addItem("Libre");
	m_lstCameraType.setCurrentItem(0);

	m_tabs.addComponent(&m_lblScene, 0);
	m_tabs.addComponent(&m_lstScene, 0);
	m_groupTransform.addComponent(&m_lblPosX);
	m_groupTransform.addComponent(&m_sldPosX);
	m_groupTransform.addComponent(&m_lblPosY);
	m_groupTransform.addComponent(&m_sldPosY);
	m_groupTransform.addComponent(&m_lblPosZ);
	m_groupTransform.addComponent(&m_sldPosZ);
	m_groupTransform.addComponent(&m_lblRotX);
	m_groupTransform.addComponent(&m_sldRotX);
	m_groupTransform.addComponent(&m_lblRotY);
	m_groupTransform.addComponent(&m_sldRotY);
	m_groupTransform.addComponent(&m_lblRotZ);
	m_groupTransform.addComponent(&m_sldRotZ);
	m_groupMesh.addComponent(&m_ckbCulling);
	m_groupMesh.addComponent(&m_lblMeshInfo);
	m_groupMesh.addComponent(&m_lblMeshVerts);
	m_groupMesh.addComponent(&m_lblNumVerts);
	m_groupMesh.addComponent(&m_lblMeshFaces);
	m_groupMesh.addComponent(&m_lblNumFaces);
	m_groupCamera.addComponent(&m_lblCameraType);
	m_groupCamera.addComponent(&m_lstCameraType);
	m_tabs.addComponent(&m_groupTransform, 0);
	m_tabs.addComponent(&m_groupMesh, 0);
	m_tabs.addComponent(&m_groupCamera, 0);

	// Render
	m_btnRender.create(100, 25, 80, 30, "Renderizar", 81);
	m_btnRender.handleClick(makeFunctor(*this, &FractureViewerGUI::OnClick));

	m_groupScene.create(15, 60, 250, 70, "Escena");
	m_ckbWireframe.create(20, 25, "Wireframe", 301);
	m_ckbWireframe.handleOnClick(makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_sldWireframe.create(110, 25, 90);
	m_sldWireframe.setBounds(0, 10, 2);
	m_sldWireframe.bindValue(m_scene->GetWireframeLineWidth());
	m_ckbGrid.create(20, 45, "Grid", 306);
	m_ckbGrid.handleOnClick(makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_ckbGrid.setState(true);
	
	m_groupSkyLight.create(15, 140, 250, 100, "SkyLight");
	m_ckbSkyLight.create(20, 15, "Visible", 305);
	m_ckbSkyLight.handleOnClick(makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_ckbSkyLight.setState(false);
	m_lblSkyLightIntensity.create(15, 35, "Intensidad SkyLight");
	m_sldSkyLightIntensity.create(15, 55, 120);
	m_sldSkyLightIntensity.setBounds(0, 5, 1);
	m_sldSkyLightIntensity.bindValue(&m_scene->GetCubeMap()->m_intensity);
	m_ckbSkyReflections.create(15, 75, "Skylight", 307);
	m_ckbSkyReflections.handleOnClick(makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_ckbSkyReflections.setState(false);
	m_groupScene.addComponent(&m_ckbWireframe);
	m_groupScene.addComponent(&m_sldWireframe);
	m_groupScene.addComponent(&m_ckbGrid);
	m_groupSkyLight.addComponent(&m_ckbSkyLight);
	m_groupSkyLight.addComponent(&m_lblSkyLightIntensity);
	m_groupSkyLight.addComponent(&m_sldSkyLightIntensity);
	m_groupSkyLight.addComponent(&m_ckbSkyReflections);

	m_groupBoundingBox.create(15, 250, 250, 250, "Bounding Box");
	m_ckbBoundingBox.create(15, 20, "Visible", 308, makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_ckbBoundingBox.setState(false);
	m_lblBoundingBox.create(80, 40, "Tipo Bounding Box");
	m_lstBoundingBox.create(20, 55, 215, 200, begui::ListBox::SINGLE_SELECT);
	m_lstBoundingBox.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectBoundingBox));
	m_lstBoundingBox.addItem("Cubo");
	m_lstBoundingBox.addItem("Capsula");
	m_lstBoundingBox.addItem("CapsulaX");
	m_lstBoundingBox.addItem("CapsulaZ");
	m_lstBoundingBox.addItem("Cilindro");
	m_lstBoundingBox.addItem("CilindroX");
	m_lstBoundingBox.addItem("CilindroZ");
	m_lstBoundingBox.addItem("Cono");
	m_lstBoundingBox.addItem("ConoX");
	m_lstBoundingBox.addItem("ConoZ");
	m_lstBoundingBox.addItem("Esfera");
	m_lstBoundingBox.addItem("Malla");
	m_lstBoundingBox.setCurrentItem(0);
	m_groupBoundingBox.addComponent(&m_ckbBoundingBox);
	m_groupBoundingBox.addComponent(&m_lblBoundingBox);
	m_groupBoundingBox.addComponent(&m_lstBoundingBox);

	m_tabs.addComponent(&m_btnRender, 1);
	m_tabs.addComponent(&m_groupScene, 1);
	m_tabs.addComponent(&m_groupSkyLight, 1);
	m_tabs.addComponent(&m_groupBoundingBox, 1);

	// Animacion
	m_groupSkeleton.create(15, 20, 250, 100, "Esqueleto");
	m_ckbJoints.create(15, 20, "Mostrar Joints", 309, makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_ckbJoints.setState(false);
	m_ckbJoints.disable();
	m_ckbSkinning.create(120, 20, "Mostar Skinning", 310, makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_ckbSkinning.setState(false);
	m_ckbSkinning.disable();
	m_groupSkeleton.addComponent(&m_ckbJoints);
	m_groupSkeleton.addComponent(&m_ckbSkinning);
	m_tabs.addComponent(&m_groupSkeleton, 2);

	// Material
	m_lblSelShader.create(20, 10, "Selecciona el material a usar ...");
	m_cbxShaders.create(15, 30, 250, 200);
	m_cbxShaders.addItem("Phong");
	m_cbxShaders.addItem("Toon");
	m_cbxShaders.disableItem(1);
	m_cbxShaders.setCurrentItem(0);
	m_groupTextures.create(15, 60, 250, 330, "Texturas");
	m_lblDiffuse.create(20, 15, "Mapa Difuso");
	m_btnDiffuse.create(195, 25, 30, 15, "...", 601, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_btnDelDiffuse.create(195, 45, 30, 15, "X", 701, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_txtDiffuse.create(15, 35, 170, 20, false, false);
	m_txtDiffuse.setText("Sin textura");
	m_lblSpecular.create(20, 65, "Mapa Especular");
	m_btnSpecular.create(195, 75, 30, 15, "...", 602, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_btnDelSpecular.create(195, 95, 30, 15, "X", 702, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_txtSpecular.create(15, 85, 170, 20, false, false);
	m_txtSpecular.setText("Sin textura");
	m_lblAmbient.create(20, 115, "Mapa Ambiente");
	m_btnAmbient.create(195, 125, 30, 15, "...", 603, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_btnDelAmbient.create(195, 145, 30, 15, "X", 703, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_txtAmbient.create(15, 135, 170, 20, false, false);
	m_txtAmbient.setText("Sin textura");
	m_lblEmissive.create(20, 165, "Mapa Emisivo");
	m_btnEmissive.create(195, 175, 30, 15, "...", 604, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_btnDelEmissive.create(195, 195, 30, 15, "X", 704, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_txtEmissive.create(15, 185, 170, 20, false, false);
	m_txtEmissive.setText("Sin textura");
	m_lblNormal.create(20, 215, "Mapa normal");
	m_btnNormal.create(195, 225, 30, 15, "...", 605, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_btnDelNormal.create(195, 245, 30, 15, "X", 705, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_txtNormal.create(15, 235, 170, 20, false, false);
	m_txtNormal.setText("Sin textura");
	m_lblAlpha.create(20, 265, "Mapa transparencia");
	m_btnAlpha.create(195, 275, 30, 15, "...", 606, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_btnDelAlpha.create(195, 295, 30, 15, "X", 706, makeFunctor(*this, &FractureViewerGUI::OnClick));
	m_txtAlpha.create(15, 285, 170, 20, false, false);
	m_txtAlpha.setText("Sin textura");
	m_lblTiling.create(10, 315, "Tiling");
	m_sldTiling.create(45, 315, 140);
	m_sldTiling.setBounds(0, 8, 1);
	m_sldTiling.bindValue(&m_scene->GetMaterial()->textureTiling);
	m_btnResTiling.create(215, 315, 15, 15, "R", 607, makeFunctor(*this, &FractureViewerGUI::OnClick));

	m_groupMatProperties.create(15, 420, 250, 220, "Propiedades");
	m_lblMatColor.create(15, 15, "Color");
	m_lblColorR.create(10, 35, "R");
	m_sldColorR.create(25, 35, 180);
	m_sldColorR.setBounds(0, 1, 0);
	m_sldColorR.bindValue(&m_scene->GetMaterial()->diffuseColor.r);
	m_lblColorG.create(10, 55, "G");
	m_sldColorG.create(25, 55, 180);
	m_sldColorG.setBounds(0, 1, 0);
	m_sldColorG.bindValue(&m_scene->GetMaterial()->diffuseColor.g);
	m_lblColorB.create(10, 75, "B");
	m_sldColorB.create(25, 75, 180);
	m_sldColorB.setBounds(0, 1, 0);
	m_sldColorB.bindValue(&m_scene->GetMaterial()->diffuseColor.b);
	m_lblSpecIntensity.create(15, 100, "Exp");
	m_sldSpecIntensity.create(55, 100, 150);
	m_sldSpecIntensity.setBounds(0, 10, 1);
	m_sldSpecIntensity.bindValue(&m_scene->GetMaterial()->specularIntensity);
	m_lblSpecPower.create(15, 120, "Gloss");
	m_sldSpecPower.create(55, 120, 150);
	m_sldSpecPower.setBounds(0, 200, 32);
	m_sldSpecPower.bindValue(&m_scene->GetMaterial()->specularPower);
	m_lblEmissionContribution.create(15, 140, "EmiC");
	m_sldEmissionContribution.create(55, 140, 150);
	m_sldEmissionContribution.setBounds(0, 1, 0.02);
	m_sldEmissionContribution.bindValue(&m_scene->GetMaterial()->emissionContribution);
	m_lblAmbientContribution.create(15, 160, "AmbC");
	m_sldAmbientContribution.create(55, 160, 150);
	m_sldAmbientContribution.setBounds(0, 1, 0.2);
	m_sldAmbientContribution.bindValue(&m_scene->GetMaterial()->ambientContribution);
	m_lblDiffuseContribution.create(15, 180, "DifC");
	m_sldDiffuseContribution.create(55, 180, 150);
	m_sldDiffuseContribution.setBounds(0, 1, 0.4);
	m_sldDiffuseContribution.bindValue(&m_scene->GetMaterial()->diffuseContribution);
	m_lblSpecularContribution.create(15, 200, "EspC");
	m_sldSpecularContribution.create(55, 200, 150);
	m_sldSpecularContribution.setBounds(0, 1, 0.38);
	m_sldSpecularContribution.bindValue(&m_scene->GetMaterial()->specularContribution);

	m_tabs.addComponent(&m_lblSelShader, 3);
	m_tabs.addComponent(&m_cbxShaders, 3);
	m_groupTextures.addComponent(&m_lblDiffuse);
	m_groupTextures.addComponent(&m_btnDiffuse);
	m_groupTextures.addComponent(&m_btnDelDiffuse);
	m_groupTextures.addComponent(&m_txtDiffuse);
	m_groupTextures.addComponent(&m_lblSpecular);
	m_groupTextures.addComponent(&m_btnSpecular);
	m_groupTextures.addComponent(&m_btnDelSpecular);
	m_groupTextures.addComponent(&m_txtSpecular);
	m_groupTextures.addComponent(&m_lblAmbient);
	m_groupTextures.addComponent(&m_btnAmbient);
	m_groupTextures.addComponent(&m_btnDelAmbient);
	m_groupTextures.addComponent(&m_txtAmbient);
	m_groupTextures.addComponent(&m_lblEmissive);
	m_groupTextures.addComponent(&m_btnEmissive);
	m_groupTextures.addComponent(&m_btnDelEmissive);
	m_groupTextures.addComponent(&m_txtEmissive);
	m_groupTextures.addComponent(&m_lblNormal);
	m_groupTextures.addComponent(&m_btnNormal);
	m_groupTextures.addComponent(&m_btnDelNormal);
	m_groupTextures.addComponent(&m_txtNormal);
	m_groupTextures.addComponent(&m_lblAlpha);
	m_groupTextures.addComponent(&m_btnAlpha);
	m_groupTextures.addComponent(&m_btnDelAlpha);
	m_groupTextures.addComponent(&m_txtAlpha);
	m_groupTextures.addComponent(&m_lblTiling);
	m_groupTextures.addComponent(&m_sldTiling);
	m_groupTextures.addComponent(&m_btnResTiling);
	m_groupMatProperties.addComponent(&m_lblMatColor);
	m_groupMatProperties.addComponent(&m_lblColorR);
	m_groupMatProperties.addComponent(&m_lblColorG);
	m_groupMatProperties.addComponent(&m_lblColorB);
	m_groupMatProperties.addComponent(&m_sldColorR);
	m_groupMatProperties.addComponent(&m_sldColorG);
	m_groupMatProperties.addComponent(&m_sldColorB);
	m_groupMatProperties.addComponent(&m_lblSpecIntensity);
	m_groupMatProperties.addComponent(&m_sldSpecIntensity);
	m_groupMatProperties.addComponent(&m_lblSpecPower);
	m_groupMatProperties.addComponent(&m_sldSpecPower);
	m_groupMatProperties.addComponent(&m_lblEmissionContribution);
	m_groupMatProperties.addComponent(&m_sldEmissionContribution);
	m_groupMatProperties.addComponent(&m_lblAmbientContribution);
	m_groupMatProperties.addComponent(&m_sldAmbientContribution);
	m_groupMatProperties.addComponent(&m_lblDiffuseContribution);
	m_groupMatProperties.addComponent(&m_sldDiffuseContribution);
	m_groupMatProperties.addComponent(&m_lblSpecularContribution);
	m_groupMatProperties.addComponent(&m_sldSpecularContribution);
	m_tabs.addComponent(&m_groupTextures, 3);
	m_tabs.addComponent(&m_groupMatProperties, 3);

	//Luces
	m_groupAmbientLight.create(15, 30, 250, 100, "Luz Ambiente");
	m_ckbAmbientLightActive.create(175, 10, "Activada", 303);
	m_ckbAmbientLightActive.setState(true);
	m_ckbAmbientLightActive.handleOnClick(makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_lblAmbientLightColor.create(15, 15, "Color");
	m_lblAmbientLightColorR.create(10, 35, "R");
	m_sldAmbientLightColorR.create(25, 35, 180);
	m_sldAmbientLightColorR.setBounds(0, 1, 0);
	m_sldAmbientLightColorR.bindValue(&m_scene->GetAmbientLight()->r);
	m_lblAmbientLightColorG.create(10, 55, "G");
	m_sldAmbientLightColorG.create(25, 55, 180);
	m_sldAmbientLightColorG.setBounds(0, 1, 0);
	m_sldAmbientLightColorG.bindValue(&m_scene->GetAmbientLight()->g);
	m_lblAmbientLightColorB.create(10, 75, "B");
	m_sldAmbientLightColorB.create(25, 75, 180);
	m_sldAmbientLightColorB.setBounds(0, 1, 0);
	m_sldAmbientLightColorB.bindValue(&m_scene->GetAmbientLight()->b);

	m_groupDirectionalLight.create(15, 150, 250, 300, "Luz Direccional");
	m_ckbDirectionalLightActive.create(175, 10, "Activada", 304);
	m_ckbDirectionalLightActive.setState(true);
	m_ckbDirectionalLightActive.handleOnClick(makeFunctor(*this, &FractureViewerGUI::OnSelectCheckBox));
	m_lblDirectionalLightDiffColor.create(15, 15, "Color Difuso");
	m_lblDirectionalLightDiffColorR.create(10, 35, "R");
	m_sldDirectionalLightDiffColorR.create(25, 35, 180);
	m_sldDirectionalLightDiffColorR.setBounds(0, 1, 0);
	m_sldDirectionalLightDiffColorR.bindValue(&m_scene->GetDirectionalLight()->base.color.r);
	m_lblDirectionalLightDiffColorG.create(10, 55, "G");
	m_sldDirectionalLightDiffColorG.create(25, 55, 180);
	m_sldDirectionalLightDiffColorG.setBounds(0, 1, 0);
	m_sldDirectionalLightDiffColorG.bindValue(&m_scene->GetDirectionalLight()->base.color.g);
	m_lblDirectionalLightDiffColorB.create(10, 75, "B");
	m_sldDirectionalLightDiffColorB.create(25, 75, 180);
	m_sldDirectionalLightDiffColorB.setBounds(0, 1, 0);
	m_sldDirectionalLightDiffColorB.bindValue(&m_scene->GetDirectionalLight()->base.color.b);
	m_lblDirectionalLightSpecColor.create(15, 95, "Color Especular");
	m_lblDirectionalLightSpecColorR.create(10, 115, "R");
	m_sldDirectionalLightSpecColorR.create(25, 115, 180);
	m_sldDirectionalLightSpecColorR.setBounds(0, 1, 0);
	m_sldDirectionalLightSpecColorR.bindValue(&m_scene->GetDirectionalLight()->base.colorSpec.r);
	m_lblDirectionalLightSpecColorG.create(10, 135, "G");
	m_sldDirectionalLightSpecColorG.create(25, 135, 180);
	m_sldDirectionalLightSpecColorG.setBounds(0, 1, 0);
	m_sldDirectionalLightSpecColorG.bindValue(&m_scene->GetDirectionalLight()->base.colorSpec.g);
	m_lblDirectionalLightSpecColorB.create(10, 155, "B");
	m_sldDirectionalLightSpecColorB.create(25, 155, 180);
	m_sldDirectionalLightSpecColorB.setBounds(0, 1, 0);
	m_sldDirectionalLightSpecColorB.bindValue(&m_scene->GetDirectionalLight()->base.colorSpec.b);
	m_lblDirectionalLightIntensity.create(15, 175, "Intensidad Luz Direccional");
	m_sldDirectionalLightIntensity.create(15, 195, 200);
	m_sldDirectionalLightIntensity.setBounds(0, 10, 1);
	m_sldDirectionalLightIntensity.bindValue(&m_scene->GetDirectionalLight()->base.intensity);
	m_lblDirectionalLightDirection.create(15, 215, "Direccion");
	m_lblDirectionalLightDirectionX.create(10, 235, "X");
	m_sldDirectionalLightDirectionX.create(25, 235, 180);
	m_sldDirectionalLightDirectionX.setBounds(-360, 360, 0);
	m_sldDirectionalLightDirectionX.bindValue(&m_scene->GetDirectionalLight()->direction.x);
	m_lblDirectionalLightDirectionY.create(10, 255, "Y");
	m_sldDirectionalLightDirectionY.create(25, 255, 180);
	m_sldDirectionalLightDirectionY.setBounds(-360, 360, 0);
	m_sldDirectionalLightDirectionY.bindValue(&m_scene->GetDirectionalLight()->direction.y);
	m_lblDirectionalLightDirectionZ.create(10, 275, "Z");
	m_sldDirectionalLightDirectionZ.create(25, 275, 180);
	m_sldDirectionalLightDirectionZ.setBounds(-360, 360, 0);
	m_sldDirectionalLightDirectionZ.bindValue(&m_scene->GetDirectionalLight()->direction.z);

	m_groupAmbientLight.addComponent(&m_lblAmbientLightColor);
	m_groupAmbientLight.addComponent(&m_ckbAmbientLightActive);
	m_groupAmbientLight.addComponent(&m_lblAmbientLightColorR);
	m_groupAmbientLight.addComponent(&m_lblAmbientLightColorG);
	m_groupAmbientLight.addComponent(&m_lblAmbientLightColorB);
	m_groupAmbientLight.addComponent(&m_sldAmbientLightColorR);
	m_groupAmbientLight.addComponent(&m_sldAmbientLightColorG);
	m_groupAmbientLight.addComponent(&m_sldAmbientLightColorB);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDiffColor);
	m_groupDirectionalLight.addComponent(&m_ckbDirectionalLightActive);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDiffColorR);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDiffColorG);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDiffColorB);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightDiffColorR);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightDiffColorG);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightDiffColorB);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightSpecColor);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightSpecColorR);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightSpecColorG);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightSpecColorB);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightSpecColorR);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightSpecColorG);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightSpecColorB);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightIntensity);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightIntensity);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDirection);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDirectionX);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightDirectionX);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDirectionY);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightDirectionY);
	m_groupDirectionalLight.addComponent(&m_lblDirectionalLightDirectionZ);
	m_groupDirectionalLight.addComponent(&m_sldDirectionalLightDirectionZ);
	m_tabs.addComponent(&m_groupAmbientLight, 4);
	m_tabs.addComponent(&m_groupDirectionalLight, 4);

	// Contenedor principal
	mainContainer.setPos(0, 0);
	mainContainer.setSize(1200, 1000);
	mainContainer.addComponent(&m_wnd1);
}

void FractureViewerGUI::ProcessMouseLeftButtonDown(int x, int y)
{
	begui::input::mouseButtonDown(x, y, MOUSE_BUTTON_LEFT);
	mainContainer.onMouseDown(x, y, MOUSE_BUTTON_LEFT);
}

void FractureViewerGUI::ProcessMouseRightButtonDown(int x, int y)
{
	begui::input::mouseButtonDown(x, y, MOUSE_BUTTON_RIGHT);
	mainContainer.onMouseDown(x, y, MOUSE_BUTTON_RIGHT);
}

void FractureViewerGUI::ProcessMouseCenterButtonDown(int x, int y)
{
	begui::input::mouseButtonDown(x, y, MOUSE_BUTTON_MIDDLE);
	mainContainer.onMouseDown(x, y, MOUSE_BUTTON_MIDDLE);
}

void FractureViewerGUI::ProcessMouseLeftButtonUp(int x, int y)
{
	begui::input::mouseButtonUp(x, y, MOUSE_BUTTON_LEFT);
	mainContainer.onMouseUp(x, y, MOUSE_BUTTON_LEFT);
}

void FractureViewerGUI::ProcessMouseRightButtonUp(int x, int y)
{
	begui::input::mouseButtonUp(x, y, MOUSE_BUTTON_RIGHT);
	mainContainer.onMouseUp(x, y, MOUSE_BUTTON_RIGHT);
}

void FractureViewerGUI::ProcessMouseCenterButtonUp(int x, int y)
{
	begui::input::mouseButtonUp(x, y, MOUSE_BUTTON_MIDDLE);
	mainContainer.onMouseUp(x, y, MOUSE_BUTTON_MIDDLE);
}

void FractureViewerGUI::ProcessMouseMotion(int x, int y)
{
	begui::input::mousePos(x, y);
	mainContainer.onMouseMove(x, y, prevx, prevy);
	prevx = x;
	prevy = y;
}

bool FractureViewerGUI::IsPointerInside(int x, int y)
{
	if (m_wnd1.isPtInside(x, y)) return true;

	return false;
}

void FractureViewerGUI::UpdateGUI()
{
	//Escena
	if (m_scene->GetMesh() != nullptr)
	{
		m_lblNumVerts.setText(std::to_string(m_scene->GetMesh()->GetMesh()->GetNumVertices()));
		m_lblNumFaces.setText(std::to_string(m_scene->GetMesh()->GetMesh()->GetNumFaces()));
	}
	else
	{
		m_lblNumVerts.setText("0");
		m_lblNumFaces.setText("0");
	}

}

void FractureViewerGUI::SetVisible(bool flag)
{
	if (flag)
	{
		m_wnd1.setVisible(true);
		m_bIsVisible = true;
	}
	else
	{
		m_wnd1.setVisible(false);
		m_bIsVisible = false;
	}
}

bool FractureViewerGUI::GetVisible()
{
	return m_bIsVisible;
}

void FractureViewerGUI::Render()
{
	if (m_window == nullptr) return;

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_window->GetWidth(), m_window->GetHeight(), 0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	begui::display::setSize(m_window->GetWidth(), m_window->GetHeight());

	mainContainer.frameUpdate();
	mainContainer.frameRender();
}

// Implementacion de las funciones callback
void FractureViewerGUI::OnButtonDown(int id)
{
	switch (id)
	{
	case 202:
		Console::print("click\n");
		break;
	}
}

void FractureViewerGUI::OnClick(int id)
{
	switch (id)
	{
		case 81:
		{
			m_scene->GenImage();
			break;
		}
		case 601:
		{
			m_dlg1.create(30, 30, 200, 300, "Textura a cargar ...");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectDiffuseTexture));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 10002, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 602:
		{
			m_dlg1.create(30, 30, 200, 300, "Textura a cargar ...");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectSpecularTexture));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 10003, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 603:
		{
			m_dlg1.create(30, 30, 200, 300, "Textura a cargar ...");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectAmbientTexture));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 10004, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 604:
		{
			m_dlg1.create(30, 30, 200, 300, "Textura a cargar ...");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectEmissiveTexture));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 10005, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 605:
		{
			m_dlg1.create(30, 30, 200, 300, "Textura a cargar ...");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectNormalTexture));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 10006, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 606:
		{
			m_dlg1.create(30, 30, 200, 300, "Textura a cargar ...");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectAlphaTexture));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 10007, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 607:
		{
			m_scene->GetMaterial()->textureTiling = 1.0f;
			break;
		}
		case 701:
		{
			m_txtDiffuse.setText("Sin Textura");
			m_diffuseTextureSelected = "";
			m_scene->SetDiffuse("");
			break;
		}
		case 702:
		{
			m_txtSpecular.setText("Sin Textura");
			m_specularTextureSelected = "";
			m_scene->SetSpecular("");
			break;
		}
		case 703:
		{
			m_txtAmbient.setText("Sin Textura");
			m_ambientTextureSelected = "";
			m_scene->SetAmbient("");
			break;
		}
		case 704:
		{
			m_txtEmissive.setText("Sin Textura");
			m_emissiveTextureSelected = "";
			m_scene->SetEmissive("");
			break;
		}
		case 705:
		{
			m_txtNormal.setText("Sin Textura");
			m_normalTextureSelected = "";
			m_scene->SetNormal("");
			FracturePhongShader::SetNormalActive(false);
			break;
		}
		case 706:
		{
			m_txtAlpha.setText("Sin Textura");
			m_alphaTextureSelected = "";
			m_scene->SetAlpha("");
			break;
		}
		case 801:
		{
			m_dlg1.create(30, 30, 200, 300, "Cubemap - NegX");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectCubeMapFaceNegX));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 1, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
			case 802:
		{
			m_dlg1.create(30, 30, 200, 300, "Cubemap - NegY");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectCubeMapFaceNegY));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 2, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 803:
		{
			m_dlg1.create(30, 30, 200, 300, "Cubemap - NegZ");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectCubeMapFaceNegZ));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 3, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 804:
		{
			m_dlg1.create(30, 30, 200, 300, "Cubemap - PosX");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectCubeMapFacePosX));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 4, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 805:
		{
			m_dlg1.create(30, 30, 200, 300, "Cubemap - PosY");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectCubeMapFacePosY));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 5, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
		case 806:
		{
			m_dlg1.create(30, 30, 200, 300, "Cubemap - PosZ");
			m_dlg1.enableClose(false);

			m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
			m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectCubeMapFacePosZ));

			std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/textures/", "*");
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				m_lst1.addItem(*it);
			}
			m_lst1.setCurrentItem(1);
			m_dlg1.addComponent(&m_lst1);

			m_dlgBtn1.create(55, 235, "Cargar", 6, makeFunctor(*this, &FractureViewerGUI::OnMenu));
			m_dlg1.addComponent(&m_dlgBtn1);
			mainContainer.showModal(&m_dlg1);
			break;
		}
	}
}

void FractureViewerGUI::OnMenu(int id)
{
	switch (id)
	{
	case 1:
	{
		mainContainer.showModal(&m_dlg2);
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_txtNegXCbp.setText(m_negX.c_str());
		}
		break;
	}
	case 2:
	{
		mainContainer.showModal(&m_dlg2);
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_txtNegYCbp.setText(m_negY.c_str());
		}
		break;
	}
		case 3:
	{
		mainContainer.showModal(&m_dlg2);
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_txtNegZCbp.setText(m_negZ.c_str());
		}
		break;
	}
	case 4:
	{
		mainContainer.showModal(&m_dlg2);
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_txtPosXCbp.setText(m_posX.c_str());
		}
		break;
	}
	case 5:
	{
		mainContainer.showModal(&m_dlg2);
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_txtPosYCbp.setText(m_posY.c_str());
		}
		break;
	}
	case 6:
	{
		mainContainer.showModal(&m_dlg2);
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_txtPosZCbp.setText(m_posZ.c_str());
		}
		break;
	}
	case 101:
	{
		m_dlg1.create(30, 30, 200, 300, "Modelo a cargar ...");
		m_dlg1.enableClose(false);

		m_lst1.create(20, 20, 160, 200, begui::ListBox::SINGLE_SELECT);
		m_lst1.setAutoHeight(false);
		m_lst1.handleOnItemSelect(makeFunctor(*this, &FractureViewerGUI::OnSelectModel));

		std::vector<std::string> files = GetAllFilesnamesWithinFolder("./res/meshes/", "*");
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			m_lst1.addItem(*it);
		}
		m_lst1.setCurrentItem(1);
		m_dlg1.addComponent(&m_lst1);

		m_dlgBtn1.create(55, 235, "Cargar", 10001, makeFunctor(*this, &FractureViewerGUI::OnMenu));
		m_dlg1.addComponent(&m_dlgBtn1);
		mainContainer.showModal(&m_dlg1);
		break;
	}
	case 102:
	{
		m_dlg2.create(30, 30, 220, 340, "Cargar cubemap ...");
		m_dlg2.enableClose(true);

		m_lblNegXCbp.create(20, 20, "NegX");
		m_lblNegYCbp.create(20, 60, "NegY");
		m_lblNegZCbp.create(20, 100, "NegZ");
		m_lblPosXCbp.create(20, 140, "PosX");
		m_lblPosYCbp.create(20, 180, "PosY");
		m_lblPosZCbp.create(20, 220, "PosZ");
		m_txtNegXCbp.create(50, 20, 110, 20, false, false);
		m_txtNegYCbp.create(50, 60, 110, 20, false, false);
		m_txtNegZCbp.create(50, 100, 110, 20, false, false);
		m_txtPosXCbp.create(50, 140, 110, 20, false, false);
		m_txtPosYCbp.create(50, 180, 110, 20, false, false);
		m_txtPosZCbp.create(50, 220, 110, 20, false, false);
		m_btnNegXCbp.create(170, 10, 20, 15, "...", 801, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnNegYCbp.create(170, 50, 20, 15, "...", 802, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnNegZCbp.create(170, 90, 20, 15, "...", 803, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnPosXCbp.create(170, 130, 20, 15, "...", 804, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnPosYCbp.create(170, 170, 20, 15, "...", 805, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnPosZCbp.create(170, 210, 20, 15, "...", 806, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnDelNegXCbp.create(170, 30, 20, 15, "X", 807, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnDelNegYCbp.create(170, 70, 20, 15, "X", 808, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnDelNegZCbp.create(170, 110, 20, 15, "X", 809, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnDelPosXCbp.create(170, 150, 20, 15, "X", 810, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnDelPosYCbp.create(170, 190, 20, 15, "X", 811, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_btnDelPosZCbp.create(170, 230, 20, 15, "X", 812, makeFunctor(*this, &FractureViewerGUI::OnClick));
		m_dlgBtn2.create(55, 270, "Cargar", 10008, makeFunctor(*this, &FractureViewerGUI::OnMenu));
		
		m_dlg2.addComponent(&m_lblNegXCbp);
		m_dlg2.addComponent(&m_lblNegYCbp);
		m_dlg2.addComponent(&m_lblNegZCbp);
		m_dlg2.addComponent(&m_lblPosXCbp);
		m_dlg2.addComponent(&m_lblPosYCbp);
		m_dlg2.addComponent(&m_lblPosZCbp);
		m_dlg2.addComponent(&m_txtNegXCbp);
		m_dlg2.addComponent(&m_txtNegYCbp);
		m_dlg2.addComponent(&m_txtNegZCbp);
		m_dlg2.addComponent(&m_txtPosXCbp);
		m_dlg2.addComponent(&m_txtPosYCbp);
		m_dlg2.addComponent(&m_txtPosZCbp);
		m_dlg2.addComponent(&m_btnNegXCbp);
		m_dlg2.addComponent(&m_btnNegYCbp);
		m_dlg2.addComponent(&m_btnNegZCbp);
		m_dlg2.addComponent(&m_btnPosXCbp);
		m_dlg2.addComponent(&m_btnPosYCbp);
		m_dlg2.addComponent(&m_btnPosZCbp);
		m_dlg2.addComponent(&m_btnDelNegXCbp);
		m_dlg2.addComponent(&m_btnDelNegYCbp);
		m_dlg2.addComponent(&m_btnDelNegZCbp);
		m_dlg2.addComponent(&m_btnDelPosXCbp);
		m_dlg2.addComponent(&m_btnDelPosYCbp);
		m_dlg2.addComponent(&m_btnDelPosZCbp);
		m_dlg2.addComponent(&m_dlgBtn2);
		mainContainer.showModal(&m_dlg2);
		break;
	}
	case 10001:
	{
		mainContainer.hideModal();
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_scene->SetMesh(m_meshSelected);
			UpdateGUI();
		}
		break;
	}
	case 10002:
	{
		mainContainer.hideModal();
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_scene->SetDiffuse(m_diffuseTextureSelected.c_str());
			m_txtDiffuse.setText(m_diffuseTextureSelected.c_str());
		}
		break;
	}
	case 10003:
	{
		mainContainer.hideModal();
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_scene->SetSpecular(m_specularTextureSelected.c_str());
			m_txtSpecular.setText(m_specularTextureSelected.c_str());
		}
		break;
	}
	case 10004:
	{
		mainContainer.hideModal();
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_scene->SetAmbient(m_ambientTextureSelected.c_str());
			m_txtAmbient.setText(m_ambientTextureSelected.c_str());
		}
		break;
	}
	case 10005:
	{
		mainContainer.hideModal();
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_scene->SetEmissive(m_emissiveTextureSelected.c_str());
			m_txtEmissive.setText(m_emissiveTextureSelected.c_str());
		}
		break;
	}
	case 10006:
	{
		mainContainer.hideModal();
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_scene->SetNormal(m_normalTextureSelected.c_str());
			m_txtNormal.setText(m_normalTextureSelected.c_str());
			FracturePhongShader::SetNormalActive(true);
		}
		break;
	}
	case 10007:
	{
		mainContainer.hideModal();
		m_lst1.remAllItems();
		if (m_lst1.getCurrentItem() != 1)
		{
			m_scene->SetAlpha(m_alphaTextureSelected.c_str());
			m_txtAlpha.setText(m_alphaTextureSelected.c_str());
		}
		break;
	}
	case 10008:
	{
		char front[256];
		char back[256];
		char top[256];
		char bottom[256];
		char left[256];
		char right[256];
		mainContainer.hideModal();
		if (m_txtNegXCbp.getText() != "" && m_txtNegYCbp.getText() != "" && m_txtNegZCbp.getText() != "" &&
			m_txtPosXCbp.getText() != "" && m_txtPosYCbp.getText() != "" && m_txtPosZCbp.getText() != "")
		{
			sprintf(front, "./res/textures/%s", m_txtNegZCbp.getText().c_str());
			sprintf(back, "./res/textures/%s", m_txtPosZCbp.getText().c_str());
			sprintf(top, "./res/textures/%s", m_txtPosYCbp.getText().c_str());
			sprintf(bottom, "./res/textures/%s", m_txtNegYCbp.getText().c_str());
			sprintf(left, "./res/textures/%s", m_txtNegXCbp.getText().c_str());
			sprintf(right, "./res/textures/%s", m_txtPosXCbp.getText().c_str());
			delete m_scene->cubeMap;
			m_scene->cubeMap = nullptr;
			m_scene->cubeMap = new FractureCubeMap(front, back, top, bottom, left, right);
		}
		break;
	}
	case 106:
	{
		exit(0);
	}
	case 201:
	{
		if (m_window->GetFullscreen()) m_window->SetFullscreen(false);
		else m_window->SetFullscreen(true);
		break;
	}
	}
}

void FractureViewerGUI::OnSelectModel(int sel)
{
	m_meshSelected = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectShader(int sel)
{
	//m_shaderSelected = m_lstShaders.itemText(sel);
}

void FractureViewerGUI::OnSelectDiffuseTexture(int sel)
{
	m_diffuseTextureSelected = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectSpecularTexture(int sel)
{
	m_specularTextureSelected = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectAmbientTexture(int sel)
{
	m_ambientTextureSelected = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectEmissiveTexture(int sel)
{
	m_emissiveTextureSelected = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectNormalTexture(int sel)
{
	m_normalTextureSelected = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectAlphaTexture(int sel)
{
	m_alphaTextureSelected = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectCubeMapFaceNegX(int sel)
{
	m_negX = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectCubeMapFaceNegY(int sel)
{
	m_negY = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectCubeMapFaceNegZ(int sel)
{
	m_negZ = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectCubeMapFacePosX(int sel)
{
	m_posX = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectCubeMapFacePosY(int sel)
{
	m_posY = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectCubeMapFacePosZ(int sel)
{
	m_posZ = m_lst1.itemText(sel);
}

void FractureViewerGUI::OnSelectCameraType(int sel)
{
	switch (sel)
	{
	case 0:
		m_scene->GetCamera()->SetCameraType(FractureCamera::CameraType::TARGET);
		break;
	case 1:
		m_scene->GetCamera()->SetCameraType(FractureCamera::CameraType::FREE);
		break;
	}
}

void FractureViewerGUI::OnSelectBoundingBox(int sel)
{
	m_scene->GetPhysics()->RemoveRigidBody(m_scene->GetMesh()->GetRigidBody());

	switch (sel)
	{
	case 0:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 0);

		break;

	case 1:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 1);

		break;

	case 2:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 2);

		break;

	case 3:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 3);

		break;

	case 4:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 4);

		break;

	case 5:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 5);

		break;

	case 6:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 6);

		break;

	case 7:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 7);

		break;

	case 8:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 8);

		break;

	case 9:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 9);

		break;

	case 10:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 10);

		break;

	case 11:

		m_scene->GetPhysics()->createRigid(0, m_scene->GetMesh(), 11);

		break;
	}
}

void FractureViewerGUI::OnSelectCheckBox(int id)
{
	switch (id)
	{
	case 301:
		m_scene->SetWireframe(m_ckbWireframe.getState());
		break;
	case 302:
		m_scene->SetCulling(m_ckbCulling.getState());
		break;
	case 303:
		m_scene->SetAmbientLight(m_ckbAmbientLightActive.getState());
		break;
	case 304:
		m_scene->SetDirectionalLight(m_ckbDirectionalLightActive.getState());
		break;
	case 305:
		m_scene->SetSkyLight(m_ckbSkyLight.getState());
		break;
	case 306:
		m_scene->SetGrid(m_ckbGrid.getState());
		break;
	case 307:
		FracturePhongShader::SetSkyLightReflections(m_ckbSkyReflections.getState());
		break;
	case 308:
		m_scene->SetBoundingBox(m_ckbBoundingBox.getState());
		break;
	case 309:
		m_scene->SetJoints(m_ckbJoints.getState());
		break;
	case 310:
		m_scene->SetSkinning(m_ckbSkinning.getState());
		break;
	}
	
}

FractureViewerGUI::~FractureViewerGUI()
{
}
