#ifndef GL_MATH_H
#define GL_MATH_H

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace n3d {
    typedef glm::vec2 vec2;
    typedef glm::vec3 vec3;
    typedef glm::vec4 vec4;
    typedef glm::mat3 mat3;
    typedef glm::mat4 mat4;
    typedef glm::quat quat;

    using glm::radians;
    using glm::cos;

    using glm::scale;
    using glm::rotate;
    using glm::translate;

    using glm::normalize;
    using glm::cross;
    using glm::inverse;
    using glm::lookAt;
    using glm::perspective;

    using glm::make_quat;
    using glm::make_vec3;
    using glm::make_vec4;
    using glm::make_mat4;
    using glm::toMat4;
    using glm::value_ptr;
}

#endif //GL_MATH_H