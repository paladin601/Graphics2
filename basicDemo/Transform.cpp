#include "Transform.h"



glm::mat4 Transform::applyAllTransform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate)
{
	glm::mat4 mat = glm::mat4(1.0f);
	mat = applyTranslate(mat, translate);
	mat = applyRotate(mat, rotate);
	mat = applyScale(mat, scale);
	return mat;
}

glm::mat4 Transform::applyScale(glm::mat4 mat, glm::vec3 scale)
{
	return glm::scale(mat, scale);
}

glm::mat4 Transform::applyRotate(glm::mat4 mat, glm::vec3 rotate)
{
	mat = glm::rotate(mat, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return mat;
}

glm::mat4 Transform::applyTranslate(glm::mat4 mat, glm::vec3 translate)
{
	return glm::translate(mat, translate);
}

Transform::Transform()
{
}


Transform::~Transform()
{
}
