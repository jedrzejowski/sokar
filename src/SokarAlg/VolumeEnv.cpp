//
// Created by adam on 05.05.2021.
//

#include "VolumeEnv.hpp"

using namespace SokarAlg;

VolumeEnv::VolumeEnv(Volume *child, const glm::i32vec3 &envSize, float envValue)
		: envSize(envSize), child(child), envValue(envValue) {
}

glm::i32vec3 VolumeEnv::getSize() const {
	return child->getSize() + envSize * 2;
}

float VolumeEnv::getValue(const glm::i32vec3 &position) const {
	auto pos = position - envSize;
	auto size = child->getSize();

	if (
			position.x >= 0 &&
			position.y >= 0 &&
			position.z >= 0 &&
			position.x <= size.x &&
			position.y <= size.y &&
			position.z <= size.z
			) {
		return child->getValue(pos);
	}

	return envValue;
}