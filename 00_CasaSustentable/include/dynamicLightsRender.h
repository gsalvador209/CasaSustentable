void dynamicLightDraw(Model* model, glm::mat4 projection, glm::mat4 view, glm::vec3 translate, \
	glm::vec3 rotate, float degree, glm::vec3 scale, int t) {


	dynamicLightsShader->use();
	dynamicLightsShader->setMat4("projection", projection);
	dynamicLightsShader->setMat4("view", view);

	// Aplicamos transformaciones del modelo
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translate); // translate it down so it's at the center of the scene
	modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), rotate);
	modelMatrix = glm::scale(modelMatrix, scale);
	dynamicLightsShader->setMat4("model", modelMatrix);

	//Afectado únicamente por la luz solar
	// Configuramos propiedades de fuentes de luz
	dynamicLightsShader->setInt("numLights", (int)gLights.size());
	for (size_t i = 0; i < gLights.size(); ++i) {
		SetLightUniformVec3(dynamicLightsShader, "Position", i, gLights[i].Position);
		SetLightUniformVec3(dynamicLightsShader, "Direction", i, gLights[i].Direction);
		SetLightUniformVec4(dynamicLightsShader, "Color", i, gLights[i].Color);
		SetLightUniformVec4(dynamicLightsShader, "Power", i, gLights[i].Power);
		SetLightUniformInt(dynamicLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
		SetLightUniformFloat(dynamicLightsShader, "distance", i, gLights[i].distance);
	}

	dynamicLightsShader->setVec3("eye", camera.Position);

	// Aplicamos propiedades materiales
	dynamicLightsShader->setVec4("MaterialAmbientColor", material01.ambient);
	dynamicLightsShader->setVec4("MaterialDiffuseColor", material01.diffuse);
	dynamicLightsShader->setVec4("MaterialSpecularColor", material01.specular);
	dynamicLightsShader->setFloat("transparency", material01.transparency);

	glm::mat4 reflex = glm::mat4(1.0f);
	dynamicLightsShader->setMat4("reflex", reflex);

	////Carga de animación
	dynamicLightsShader->setInt("frame", t);

	model->Draw(*dynamicLightsShader);
	glUseProgram(0);
}