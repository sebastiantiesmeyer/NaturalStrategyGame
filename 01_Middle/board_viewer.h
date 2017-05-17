#pragma once
#include "data_types.h"
#include "imgui\imgui.h"

//four corners of the table have different colors, cell color computed as their bilinear interpolation
const glm::vec4 base00 = glm::vec4(0.12f, 0.38f, 0.91f, 1.00f); //top-left ...
const glm::vec4 base01 = glm::vec4(0.26f, 0.85f, 0.10f, 1.00f);
const glm::vec4 base10 = glm::vec4(0.79f, 0.12f, 0.34f, 1.00f);
const glm::vec4 base11 = glm::vec4(0.76f, 0.10f, 0.86f, 1.00f); //button-right
//button state (multiplicative)
const glm::vec4 normal = glm::vec4(0.85f, 0.85f, 0.85f, 0.65f);
const glm::vec4 hoover = glm::vec4(0.85f, 0.95f, 1.00f, 0.75f);
const glm::vec4 active = glm::vec4(1.00f, 0.95f, 1.00f, 1.00f);
//on selecting a unit
const glm::vec4 neighb = glm::vec4(0.20f, 0.20f, 0.20f, 0.20f); // Neighbour highlight(additive)
const glm::vec4 darken = glm::vec4(0.70f, 0.70f, 0.70f, 0.70f); // Darkening others (multiplicative)
const glm::vec4 selctd = glm::vec4(0.20f, 0.20f, 0.20f, 0.20f); // Highlight selected (additive)
//faction (additive)
const glm::vec4 fridly = glm::vec4(1.00f, 0.00f, 0.00f, 1.00f);
const glm::vec4 enemys = glm::vec4(0.00f, 0.00f, 1.00f, 1.00f);
const glm::vec4 naturl = glm::vec4(0.00f, 1.00f, 0.00f, 1.00f);
//Unit names
static const char* RSP[3] = {"R", "S", "P" };

void view_board(const Board &board, int player);

void view_board_and_add_command(const Board &board, CommandQueue &queue, int player);

void command_editor(CommandQueue &queue, const Units &units, const Board &board, int player);
