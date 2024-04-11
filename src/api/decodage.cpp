#include <iostream>
#include <msgpack.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

std::string decode64(const std::string &val) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    return std::string(It(std::begin(val)), It(std::end(val)));
}

int main() {
    std::string encoded = "halzZW5zb3JfaWSmNzQ2MzEyrnNlbnNvcl92ZXJzaW9upUZSLXY4qHBsYW50X2lkzgAAAAGkdGltZbQyMDI0LTAzLTI0VDE4OjIxOjI4WqhtZWFzdXJlc4KrdGVtcGVyYXR1cmWlMTLCsEOoaHVtaWRpdGWjMTIl";
    std::string decoded = decode64(encoded);

    msgpack::object_handle oh = msgpack::unpack(decoded.data(), decoded.size());
    msgpack::object deserialized = oh.get();

    std::cout << deserialized << std::endl;

    return 0;
}