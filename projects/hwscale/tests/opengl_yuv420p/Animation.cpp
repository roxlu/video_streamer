#include "Animation.h"
#include <assert.h>
#include <cmath>
#include <tinylib/tinylib.h>
#include <iostream>

Animation::Animation() {
}

Animation::~Animation() {
}

bool Animation::setup(int w, int h) {
  

  rx_perspective(45.0f, 1024.0f/768.0f, 0.1f, 10.0f, pm);

  vert = rx_create_shader(GL_VERTEX_SHADER, ANIM_VS);
  frag = rx_create_shader(GL_FRAGMENT_SHADER, ANIM_FS);

  prog = rx_create_program(vert, frag);
  glBindAttribLocation(prog, 0, "a_pos");
  glBindAttribLocation(prog, 1, "v_norm");
  glLinkProgram(prog);
  rx_print_shader_link_info(prog);

  glUseProgram(prog);
  glUniformMatrix4fv(glGetUniformLocation(prog, "u_pm"), 1, GL_FALSE, pm);

  u_mm = glGetUniformLocation(prog, "u_mm");
  if(u_mm < 0) {
    return false;
  }

#if defined(USE_APPLE_VAO)
  glGenVertexArraysAPPLE(1, &vao);
  glBindVertexArrayAPPLE(vao);
#else
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
#endif 

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  nvertices = 1440;
  float vertices[] = {0.000,-0.754,0.000,0.102,-0.315,-0.944,0.321,-0.642,0.233,0.102,-0.315,-0.944,-0.123,-0.642,0.377,0.102,-0.315,-0.944,0.546,-0.337,0.397,0.700,-0.268,-0.662,0.321,-0.642,0.233,0.700,-0.268,-0.662,0.642,-0.397,0.000,0.700,-0.268,-0.662,0.000,-0.754,0.000,-0.268,-0.195,-0.944,-0.123,-0.642,0.377,-0.268,-0.195,-0.944,-0.397,-0.642,0.000,-0.268,-0.195,-0.944,0.000,-0.754,0.000,-0.268,0.195,-0.944,-0.397,-0.642,0.000,-0.268,0.195,-0.944,-0.123,-0.642,-0.377,-0.268,0.195,-0.944,0.000,-0.754,0.000,0.102,0.315,-0.944,-0.123,-0.642,-0.377,0.102,0.315,-0.944,0.321,-0.642,-0.233,0.102,0.315,-0.944,0.546,-0.337,0.397,0.905,-0.268,-0.330,0.642,-0.397,0.000,0.905,-0.268,-0.330,0.717,0.000,0.233,0.905,-0.268,-0.330,-0.209,-0.337,0.642,0.025,-0.944,-0.330,0.198,-0.397,0.610,0.025,-0.944,-0.330,0.000,0.000,0.754,0.025,-0.944,-0.330,-0.675,-0.337,0.000,-0.890,-0.315,-0.330,-0.519,-0.397,0.377,-0.890,-0.315,-0.330,-0.717,0.000,0.233,-0.890,-0.315,-0.330,-0.209,-0.337,-0.642,-0.575,0.749,-0.330,-0.519,-0.397,-0.377,-0.575,0.749,-0.330,-0.443,-0.000,-0.610,-0.575,0.749,-0.330,0.546,-0.337,-0.397,0.535,0.778,-0.330,0.198,-0.397,-0.610,0.535,0.778,-0.330,0.443,-0.000,-0.610,0.535,0.778,-0.330,0.546,-0.337,0.397,0.803,-0.583,-0.126,0.717,0.000,0.233,0.803,-0.583,-0.126,0.443,0.000,0.610,0.803,-0.583,-0.126,-0.209,-0.337,0.642,-0.307,-0.944,-0.126,0.000,0.000,0.754,-0.307,-0.944,-0.126,-0.443,0.000,0.610,-0.307,-0.944,-0.126,-0.675,-0.337,0.000,-0.992,-0.000,-0.126,-0.717,0.000,0.233,-0.992,-0.000,-0.126,-0.717,-0.000,-0.233,-0.992,-0.000,-0.126,-0.209,-0.337,-0.642,-0.307,0.944,-0.126,-0.443,-0.000,-0.610,-0.307,0.944,-0.126,0.000,-0.000,-0.754,-0.307,0.944,-0.126,0.546,-0.337,-0.397,0.803,0.583,-0.126,0.443,-0.000,-0.610,0.803,0.583,-0.126,0.717,-0.000,-0.233,0.803,0.583,-0.126,0.209,0.337,0.642,0.409,-0.628,0.662,0.519,0.397,0.377,0.409,-0.628,0.662,0.123,0.642,0.377,0.409,-0.628,0.662,-0.546,0.337,0.397,-0.471,-0.583,0.662,-0.198,0.397,0.610,-0.471,-0.583,0.662,-0.321,0.642,0.233,-0.471,-0.583,0.662,-0.546,0.337,-0.397,-0.700,0.268,0.662,-0.642,0.397,-0.000,-0.700,0.268,0.662,-0.321,0.642,-0.233,-0.700,0.268,0.662,0.209,0.337,-0.642,0.039,0.749,0.662,-0.198,0.397,-0.610,0.039,0.749,0.662,0.123,0.642,-0.377,0.039,0.749,0.662,0.675,0.337,-0.000,0.724,0.195,0.662,0.519,0.397,-0.377,0.724,0.195,0.662,0.397,0.642,-0.000,0.724,0.195,0.662,-0.123,-0.642,0.377,-0.039,-0.749,-0.662,0.198,-0.397,0.610,-0.039,-0.749,-0.662,-0.209,-0.337,0.642,-0.039,-0.749,-0.662,-0.123,-0.642,0.377,0.188,-0.577,-0.795,0.321,-0.642,0.233,0.188,-0.577,-0.795,0.198,-0.397,0.610,0.188,-0.577,-0.795,0.321,-0.642,0.233,0.471,-0.583,-0.662,0.546,-0.337,0.397,0.471,-0.583,-0.662,0.198,-0.397,0.610,0.471,-0.583,-0.662,0.642,-0.397,0.000,0.700,0.268,-0.662,0.321,-0.642,-0.233,0.700,0.268,-0.662,0.546,-0.337,-0.397,0.700,0.268,-0.662,0.642,-0.397,0.000,0.607,0.000,-0.795,0.321,-0.642,0.233,0.607,0.000,-0.795,0.321,-0.642,-0.233,0.607,0.000,-0.795,0.321,-0.642,0.233,0.331,0.000,-0.944,0.000,-0.754,0.000,0.331,0.000,-0.944,0.321,-0.642,-0.233,0.331,0.000,-0.944,-0.397,-0.642,0.000,-0.724,-0.195,-0.662,-0.519,-0.397,0.377,-0.724,-0.195,-0.662,-0.675,-0.337,0.000,-0.724,-0.195,-0.662,-0.397,-0.642,0.000,-0.491,-0.357,-0.795,-0.123,-0.642,0.377,-0.491,-0.357,-0.795,-0.519,-0.397,0.377,-0.491,-0.357,-0.795,-0.123,-0.642,0.377,-0.409,-0.628,-0.662,-0.209,-0.337,0.642,-0.409,-0.628,-0.662,-0.519,-0.397,0.377,-0.409,-0.628,-0.662,-0.123,-0.642,-0.377,-0.409,0.628,-0.662,-0.519,-0.397,-0.377,-0.409,0.628,-0.662,-0.209,-0.337,-0.642,-0.409,0.628,-0.662,-0.123,-0.642,-0.377,-0.491,0.357,-0.795,-0.397,-0.642,0.000,-0.491,0.357,-0.795,-0.519,-0.397,-0.377,-0.491,0.357,-0.795,-0.397,-0.642,0.000,-0.724,0.195,-0.662,-0.675,-0.337,0.000,-0.724,0.195,-0.662,-0.519,-0.397,-0.377,-0.724,0.195,-0.662,0.321,-0.642,-0.233,0.471,0.583,-0.662,0.198,-0.397,-0.610,0.471,0.583,-0.662,0.546,-0.337,-0.397,0.471,0.583,-0.662,0.321,-0.642,-0.233,0.188,0.577,-0.795,-0.123,-0.642,-0.377,0.188,0.577,-0.795,0.198,-0.397,-0.610,0.188,0.577,-0.795,-0.123,-0.642,-0.377,-0.039,0.749,-0.662,-0.209,-0.337,-0.642,-0.039,0.749,-0.662,0.198,-0.397,-0.610,-0.039,0.749,-0.662,0.717,0.000,0.233,0.992,0.000,0.126,0.717,-0.000,-0.233,0.992,0.000,0.126,0.675,0.337,-0.000,0.992,0.000,0.126,0.717,0.000,0.233,0.982,0.000,-0.188,0.642,-0.397,0.000,0.982,0.000,-0.188,0.717,-0.000,-0.233,0.982,0.000,-0.188,0.642,-0.397,0.000,0.905,0.268,-0.330,0.546,-0.337,-0.397,0.905,0.268,-0.330,0.717,-0.000,-0.233,0.905,0.268,-0.330,0.000,0.000,0.754,0.307,-0.944,0.126,0.443,0.000,0.610,0.307,-0.944,0.126,0.209,0.337,0.642,0.307,-0.944,0.126,0.000,0.000,0.754,0.304,-0.934,-0.188,0.198,-0.397,0.610,0.304,-0.934,-0.188,0.443,0.000,0.610,0.304,-0.934,-0.188,0.198,-0.397,0.610,0.535,-0.778,-0.330,0.546,-0.337,0.397,0.535,-0.778,-0.330,0.443,0.000,0.610,0.535,-0.778,-0.330,-0.717,0.000,0.233,-0.803,-0.583,0.126,-0.443,0.000,0.610,-0.803,-0.583,0.126,-0.546,0.337,0.397,-0.803,-0.583,0.126,-0.717,0.000,0.233,-0.795,-0.577,-0.188,-0.519,-0.397,0.377,-0.795,-0.577,-0.188,-0.443,0.000,0.610,-0.795,-0.577,-0.188,-0.519,-0.397,0.377,-0.575,-0.749,-0.330,-0.209,-0.337,0.642,-0.575,-0.749,-0.330,-0.443,0.000,0.610,-0.575,-0.749,-0.330,-0.443,-0.000,-0.610,-0.803,0.583,0.126,-0.717,-0.000,-0.233,-0.803,0.583,0.126,-0.546,0.337,-0.397,-0.803,0.583,0.126,-0.443,-0.000,-0.610,-0.795,0.577,-0.188,-0.519,-0.397,-0.377,-0.795,0.577,-0.188,-0.717,-0.000,-0.233,-0.795,0.577,-0.188,-0.519,-0.397,-0.377,-0.890,0.315,-0.330,-0.675,-0.337,0.000,-0.890,0.315,-0.330,-0.717,-0.000,-0.233,-0.890,0.315,-0.330,0.443,-0.000,-0.610,0.307,0.944,0.126,0.000,-0.000,-0.754,0.307,0.944,0.126,0.209,0.337,-0.642,0.307,0.944,0.126,0.443,-0.000,-0.610,0.304,0.934,-0.188,0.198,-0.397,-0.610,0.304,0.934,-0.188,0.000,-0.000,-0.754,0.304,0.934,-0.188,0.198,-0.397,-0.610,0.025,0.944,-0.330,-0.209,-0.337,-0.642,0.025,0.944,-0.330,0.000,-0.000,-0.754,0.025,0.944,-0.330,0.443,0.000,0.610,0.575,-0.749,0.330,0.519,0.397,0.377,0.575,-0.749,0.330,0.209,0.337,0.642,0.575,-0.749,0.330,0.443,0.000,0.610,0.795,-0.577,0.188,0.717,0.000,0.233,0.795,-0.577,0.188,0.519,0.397,0.377,0.795,-0.577,0.188,0.717,0.000,0.233,0.890,-0.315,0.330,0.675,0.337,-0.000,0.890,-0.315,0.330,0.519,0.397,0.377,0.890,-0.315,0.330,-0.443,0.000,0.610,-0.535,-0.778,0.330,-0.198,0.397,0.610,-0.535,-0.778,0.330,-0.546,0.337,0.397,-0.535,-0.778,0.330,-0.443,0.000,0.610,-0.304,-0.934,0.188,0.000,0.000,0.754,-0.304,-0.934,0.188,-0.198,0.397,0.610,-0.304,-0.934,0.188,0.000,0.000,0.754,-0.025,-0.944,0.330,0.209,0.337,0.642,-0.025,-0.944,0.330,-0.198,0.397,0.610,-0.025,-0.944,0.330,-0.717,-0.000,-0.233,-0.905,0.268,0.330,-0.642,0.397,-0.000,-0.905,0.268,0.330,-0.546,0.337,-0.397,-0.905,0.268,0.330,-0.717,-0.000,-0.233,-0.982,0.000,0.188,-0.717,0.000,0.233,-0.982,0.000,0.188,-0.642,0.397,-0.000,-0.982,0.000,0.188,-0.717,0.000,0.233,-0.905,-0.268,0.330,-0.546,0.337,0.397,-0.905,-0.268,0.330,-0.642,0.397,-0.000,-0.905,-0.268,0.330,0.000,-0.000,-0.754,-0.025,0.944,0.330,-0.198,0.397,-0.610,-0.025,0.944,0.330,0.209,0.337,-0.642,-0.025,0.944,0.330,0.000,-0.000,-0.754,-0.304,0.934,0.188,-0.443,-0.000,-0.610,-0.304,0.934,0.188,-0.198,0.397,-0.610,-0.304,0.934,0.188,-0.443,-0.000,-0.610,-0.535,0.778,0.330,-0.546,0.337,-0.397,-0.535,0.778,0.330,-0.198,0.397,-0.610,-0.535,0.778,0.330,0.717,-0.000,-0.233,0.890,0.315,0.330,0.519,0.397,-0.377,0.890,0.315,0.330,0.675,0.337,-0.000,0.890,0.315,0.330,0.717,-0.000,-0.233,0.795,0.577,0.188,0.443,-0.000,-0.610,0.795,0.577,0.188,0.519,0.397,-0.377,0.795,0.577,0.188,0.443,-0.000,-0.610,0.575,0.749,0.330,0.209,0.337,-0.642,0.575,0.749,0.330,0.519,0.397,-0.377,0.575,0.749,0.330,0.123,0.642,0.377,0.268,-0.195,0.944,0.397,0.642,-0.000,0.268,-0.195,0.944,0.000,0.754,-0.000,0.268,-0.195,0.944,0.123,0.642,0.377,0.491,-0.357,0.795,0.519,0.397,0.377,0.491,-0.357,0.795,0.397,0.642,-0.000,0.491,-0.357,0.795,0.519,0.397,0.377,0.724,-0.195,0.662,0.675,0.337,-0.000,0.724,-0.195,0.662,0.397,0.642,-0.000,0.724,-0.195,0.662,-0.321,0.642,0.233,-0.102,-0.315,0.944,0.123,0.642,0.377,-0.102,-0.315,0.944,0.000,0.754,-0.000,-0.102,-0.315,0.944,-0.321,0.642,0.233,-0.188,-0.577,0.795,-0.198,0.397,0.610,-0.188,-0.577,0.795,0.123,0.642,0.377,-0.188,-0.577,0.795,-0.198,0.397,0.610,0.039,-0.749,0.662,0.209,0.337,0.642,0.039,-0.749,0.662,0.123,0.642,0.377,0.039,-0.749,0.662,-0.321,0.642,-0.233,-0.331,0.000,0.944,-0.321,0.642,0.233,-0.331,0.000,0.944,0.000,0.754,-0.000,-0.331,0.000,0.944,-0.321,0.642,-0.233,-0.607,0.000,0.795,-0.642,0.397,-0.000,-0.607,0.000,0.795,-0.321,0.642,0.233,-0.607,0.000,0.795,-0.642,0.397,-0.000,-0.700,-0.268,0.662,-0.546,0.337,0.397,-0.700,-0.268,0.662,-0.321,0.642,0.233,-0.700,-0.268,0.662,0.123,0.642,-0.377,-0.102,0.315,0.944,-0.321,0.642,-0.233,-0.102,0.315,0.944,0.000,0.754,-0.000,-0.102,0.315,0.944,0.123,0.642,-0.377,-0.188,0.577,0.795,-0.198,0.397,-0.610,-0.188,0.577,0.795,-0.321,0.642,-0.233,-0.188,0.577,0.795,-0.198,0.397,-0.610,-0.471,0.583,0.662,-0.546,0.337,-0.397,-0.471,0.583,0.662,-0.321,0.642,-0.233,-0.471,0.583,0.662,0.397,0.642,-0.000,0.268,0.195,0.944,0.123,0.642,-0.377,0.268,0.195,0.944,0.000,0.754,-0.000,0.268,0.195,0.944,0.397,0.642,-0.000,0.491,0.357,0.795,0.519,0.397,-0.377,0.491,0.357,0.795,0.123,0.642,-0.377,0.491,0.357,0.795,0.519,0.397,-0.377,0.409,0.628,0.662,0.209,0.337,-0.642,0.409,0.628,0.662,0.123,0.642,-0.377,0.409,0.628,0.662};
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // norm
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)12);

  return true;
}

void Animation::draw() {
  glUseProgram(prog);

#if defined(USE_APPLE_VAO)
  glBindVertexArrayAPPLE(vao);
#else
  glBindVertexArray(vao);
#endif

  static float t = 0.0;
  t += 0.01;
  rx_rotation(t, 0.0, 1.0, 0.0, mm);
  rx_translate(0,0,-2.0f, mm);

  glUniformMatrix4fv(u_mm, 1, GL_FALSE, mm);
  glDrawArrays(GL_POINTS, 0, nvertices);
  glDrawArrays(GL_TRIANGLES, 0, nvertices);
}

