#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

struct wavefront_vertex
{
	int idx_position;
	int idx_texcoord;
	int idx_normal;
};

struct wavefront_triangle
{
	wavefront_vertex vertices[3];
};

struct wavefront_file
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<wavefront_triangle> triangles;
};

std::unique_ptr<wavefront_file> wavefront_load( std::string const& path );