#include "internal/GeoJSONParser.hpp"

#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace persistence::internal {

namespace {

class JsonValue
{
public:
    using Object = std::vector<std::pair<std::string, JsonValue>>;
    using Array = std::vector<JsonValue>;

    using Value = std::variant<
        std::nullptr_t,
        bool,
        double,
        std::string,
        Array,
        Object
    >;

    JsonValue() = default;

    explicit JsonValue(Value value)
        : value_(std::move(value))
    {
    }

    [[nodiscard]]
    bool isNull() const noexcept
    {
        return std::holds_alternative<std::nullptr_t>(value_);
    }

    [[nodiscard]]
    bool isBool() const noexcept
    {
        return std::holds_alternative<bool>(value_);
    }

    [[nodiscard]]
    bool isNumber() const noexcept
    {
        return std::holds_alternative<double>(value_);
    }

    [[nodiscard]]
    bool isString() const noexcept
    {
        return std::holds_alternative<std::string>(value_);
    }

    [[nodiscard]]
    bool isArray() const noexcept
    {
        return std::holds_alternative<Array>(value_);
    }

    [[nodiscard]]
    bool isObject() const noexcept
    {
        return std::holds_alternative<Object>(value_);
    }

    [[nodiscard]]
    const std::string& string() const
    {
        return std::get<std::string>(value_);
    }

    [[nodiscard]]
    double number() const
    {
        return std::get<double>(value_);
    }

    [[nodiscard]]
    const Array& array() const
    {
        return std::get<Array>(value_);
    }

    [[nodiscard]]
    const Object& object() const
    {
        return std::get<Object>(value_);
    }

    [[nodiscard]]
    const JsonValue* find(std::string_view key) const
    {
        if (!isObject()) {
            return nullptr;
        }

        for (const auto& [name, value] : object()) {
            if (name == key) {
                return &value;
            }
        }

        return nullptr;
    }

private:
    Value value_{nullptr};
};


class JsonParser
{
public:
    explicit JsonParser(std::string_view input)
        : input_(input)
    {
    }

    [[nodiscard]]
    JsonValue parse()
    {
        skipWhitespace();

        JsonValue value = parseValue();

        skipWhitespace();

        if (!atEnd()) {
            throw std::runtime_error(
                "GeoJSONParser: unexpected data after JSON document");
        }

        return value;
    }

private:
    [[nodiscard]]
    bool atEnd() const noexcept
    {
        return position_ >= input_.size();
    }

    [[nodiscard]]
    char current() const
    {
        if (atEnd()) {
            throw std::runtime_error(
                "GeoJSONParser: unexpected end of JSON");
        }

        return input_[position_];
    }

    char consume()
    {
        const char character = current();
        ++position_;
        return character;
    }

    void skipWhitespace()
    {
        while (!atEnd() &&
               std::isspace(
                   static_cast<unsigned char>(
                       input_[position_]))) {
            ++position_;
        }
    }

    void expect(char expected)
    {
        if (consume() != expected) {
            throw std::runtime_error(
                "GeoJSONParser: unexpected JSON token");
        }
    }

    [[nodiscard]]
    JsonValue parseValue()
    {
        skipWhitespace();

        if (atEnd()) {
            throw std::runtime_error(
                "GeoJSONParser: missing JSON value");
        }

        switch (current()) {
        case '{':
            return parseObject();

        case '[':
            return parseArray();

        case '"':
            return JsonValue(parseString());

        case 't':
            parseLiteral("true");
            return JsonValue(true);

        case 'f':
            parseLiteral("false");
            return JsonValue(false);

        case 'n':
            parseLiteral("null");
            return JsonValue(nullptr);

        default:
            if (current() == '-' ||
                std::isdigit(
                    static_cast<unsigned char>(current()))) {
                return JsonValue(parseNumber());
            }
        }

        throw std::runtime_error(
            "GeoJSONParser: invalid JSON value");
    }

    [[nodiscard]]
    JsonValue parseObject()
    {
        expect('{');

        JsonValue::Object object;

        skipWhitespace();

        if (!atEnd() && current() == '}') {
            consume();
            return JsonValue(std::move(object));
        }

        while (true) {
            skipWhitespace();

            if (atEnd() || current() != '"') {
                throw std::runtime_error(
                    "GeoJSONParser: object key must be a string");
            }

            std::string key = parseString();

            skipWhitespace();
            expect(':');

            JsonValue value = parseValue();

            object.emplace_back(
                std::move(key),
                std::move(value));

            skipWhitespace();

            const char separator = consume();

            if (separator == '}') {
                break;
            }

            if (separator != ',') {
                throw std::runtime_error(
                    "GeoJSONParser: invalid object separator");
            }
        }

        return JsonValue(std::move(object));
    }

    [[nodiscard]]
    JsonValue parseArray()
    {
        expect('[');

        JsonValue::Array array;

        skipWhitespace();

        if (!atEnd() && current() == ']') {
            consume();
            return JsonValue(std::move(array));
        }

        while (true) {
            array.push_back(parseValue());

            skipWhitespace();

            const char separator = consume();

            if (separator == ']') {
                break;
            }

            if (separator != ',') {
                throw std::runtime_error(
                    "GeoJSONParser: invalid array separator");
            }
        }

        return JsonValue(std::move(array));
    }

    [[nodiscard]]
    std::string parseString()
    {
        expect('"');

        std::string result;

        while (!atEnd()) {
            const char character = consume();

            if (character == '"') {
                return result;
            }

            if (character != '\\') {
                result.push_back(character);
                continue;
            }

            if (atEnd()) {
                throw std::runtime_error(
                    "GeoJSONParser: invalid string escape");
            }

            const char escaped = consume();

            switch (escaped) {
            case '"':
                result.push_back('"');
                break;

            case '\\':
                result.push_back('\\');
                break;

            case '/':
                result.push_back('/');
                break;

            case 'b':
                result.push_back('\b');
                break;

            case 'f':
                result.push_back('\f');
                break;

            case 'n':
                result.push_back('\n');
                break;

            case 'r':
                result.push_back('\r');
                break;

            case 't':
                result.push_back('\t');
                break;

            case 'u':
                parseUnicodeEscape(result);
                break;

            default:
                throw std::runtime_error(
                    "GeoJSONParser: unsupported string escape");
            }
        }

        throw std::runtime_error(
            "GeoJSONParser: unterminated JSON string");
    }

    void parseUnicodeEscape(std::string& output)
    {
        if (position_ + 4 > input_.size()) {
            throw std::runtime_error(
                "GeoJSONParser: incomplete Unicode escape");
        }

        unsigned int codePoint = 0;

        for (int i = 0; i < 4; ++i) {
            const char character = input_[position_++];

            codePoint <<= 4;

            if (character >= '0' &&
                character <= '9') {
                codePoint +=
                    static_cast<unsigned int>(
                        character - '0');
            }
            else if (character >= 'a' &&
                     character <= 'f') {
                codePoint +=
                    static_cast<unsigned int>(
                        character - 'a' + 10);
            }
            else if (character >= 'A' &&
                     character <= 'F') {
                codePoint +=
                    static_cast<unsigned int>(
                        character - 'A' + 10);
            }
            else {
                throw std::runtime_error(
                    "GeoJSONParser: invalid Unicode escape");
            }
        }

        appendUtf8(output, codePoint);
    }

    static void appendUtf8(
        std::string& output,
        unsigned int codePoint)
    {
        if (codePoint <= 0x7F) {
            output.push_back(
                static_cast<char>(codePoint));
        }
        else if (codePoint <= 0x7FF) {
            output.push_back(
                static_cast<char>(
                    0xC0 | (codePoint >> 6)));

            output.push_back(
                static_cast<char>(
                    0x80 | (codePoint & 0x3F)));
        }
        else {
            output.push_back(
                static_cast<char>(
                    0xE0 | (codePoint >> 12)));

            output.push_back(
                static_cast<char>(
                    0x80 |
                    ((codePoint >> 6) & 0x3F)));

            output.push_back(
                static_cast<char>(
                    0x80 |
                    (codePoint & 0x3F)));
        }
    }

    void parseLiteral(std::string_view literal)
    {
        if (input_.substr(position_, literal.size()) != literal) {
            throw std::runtime_error(
                "GeoJSONParser: invalid JSON literal");
        }

        position_ += literal.size();
    }

    [[nodiscard]]
    double parseNumber()
    {
        const std::size_t start = position_;

        if (current() == '-') {
            ++position_;
        }

        if (atEnd()) {
            throw std::runtime_error(
                "GeoJSONParser: invalid number");
        }

        if (current() == '0') {
            ++position_;
        }
        else {
            if (!std::isdigit(
                    static_cast<unsigned char>(current()))) {
                throw std::runtime_error(
                    "GeoJSONParser: invalid number");
            }

            while (!atEnd() &&
                   std::isdigit(
                       static_cast<unsigned char>(
                           current()))) {
                ++position_;
            }
        }

        if (!atEnd() && current() == '.') {
            ++position_;

            if (atEnd() ||
                !std::isdigit(
                    static_cast<unsigned char>(current()))) {
                throw std::runtime_error(
                    "GeoJSONParser: invalid decimal number");
            }

            while (!atEnd() &&
                   std::isdigit(
                       static_cast<unsigned char>(
                           current()))) {
                ++position_;
            }
        }

        if (!atEnd() &&
            (current() == 'e' ||
             current() == 'E')) {
            ++position_;

            if (!atEnd() &&
                (current() == '+' ||
                 current() == '-')) {
                ++position_;
            }

            if (atEnd() ||
                !std::isdigit(
                    static_cast<unsigned char>(current()))) {
                throw std::runtime_error(
                    "GeoJSONParser: invalid exponent");
            }

            while (!atEnd() &&
                   std::isdigit(
                       static_cast<unsigned char>(
                           current()))) {
                ++position_;
            }
        }

        const std::string number(
            input_.substr(
                start,
                position_ - start));

        try {
            return std::stod(number);
        }
        catch (const std::exception&) {
            throw std::runtime_error(
                "GeoJSONParser: invalid numeric value");
        }
    }

private:
    std::string_view input_;
    std::size_t position_{};
};


[[nodiscard]]
std::string readFile(
    const std::filesystem::path& file)
{
    std::ifstream input(file, std::ios::binary);

    if (!input) {
        throw std::runtime_error(
            "GeoJSONParser: cannot open file: " +
            file.string());
    }

    input.seekg(0, std::ios::end);

    const auto size = input.tellg();

    if (size < 0) {
        throw std::runtime_error(
            "GeoJSONParser: cannot determine file size");
    }

    std::string content;

    content.resize(
        static_cast<std::size_t>(size));

    input.seekg(0, std::ios::beg);

    input.read(
        content.data(),
        static_cast<std::streamsize>(
            content.size()));

    if (!input && !input.eof()) {
        throw std::runtime_error(
            "GeoJSONParser: failed while reading file");
    }

    return content;
}


[[nodiscard]]
const JsonValue*
findObjectValue(
    const JsonValue& object,
    std::string_view key)
{
    return object.find(key);
}


[[nodiscard]]
std::string
readStringProperty(
    const JsonValue& object,
    std::string_view key)
{
    const auto* value =
        findObjectValue(object, key);

    if (value == nullptr ||
        !value->isString()) {
        return {};
    }

    return value->string();
}


[[nodiscard]]
bool
readCoordinate(
    const JsonValue& value,
    Coordinate& coordinate)
{
    if (!value.isArray()) {
        return false;
    }

    const auto& values = value.array();

    if (values.size() < 2 ||
        !values[0].isNumber() ||
        !values[1].isNumber()) {
        return false;
    }

    coordinate.longitude = values[0].number();
    coordinate.latitude = values[1].number();

    return true;
}


[[nodiscard]]
bool
parseLineString(
    const JsonValue& coordinates,
    std::vector<Coordinate>& line)
{
    if (!coordinates.isArray()) {
        return false;
    }

    for (const auto& value : coordinates.array()) {
        Coordinate coordinate;

        if (!readCoordinate(value, coordinate)) {
            return false;
        }

        line.push_back(coordinate);
    }

    return line.size() >= 2;
}


[[nodiscard]]
bool
parseMultiLineString(
    const JsonValue& coordinates,
    std::vector<std::vector<Coordinate>>& lines)
{
    if (!coordinates.isArray()) {
        return false;
    }

    for (const auto& lineValue : coordinates.array()) {
        std::vector<Coordinate> line;

        if (!parseLineString(lineValue, line)) {
            return false;
        }

        lines.push_back(std::move(line));
    }

    return !lines.empty();
}


[[nodiscard]]
bool
parseGeometry(
    const JsonValue& geometry,
    ParsedFeature& feature)
{
    if (!geometry.isObject()) {
        return false;
    }

    const auto* typeValue =
        geometry.find("type");

    const auto* coordinatesValue =
        geometry.find("coordinates");

    if (typeValue == nullptr ||
        !typeValue->isString() ||
        coordinatesValue == nullptr) {
        return false;
    }

    const auto& type = typeValue->string();

    if (type == "LineString") {
        std::vector<Coordinate> line;

        if (!parseLineString(
                *coordinatesValue,
                line)) {
            return false;
        }

        feature.geometryType =
            GeometryType::LineString;

        feature.lines.push_back(
            std::move(line));

        return true;
    }

    if (type == "MultiLineString") {
        if (!parseMultiLineString(
                *coordinatesValue,
                feature.lines)) {
            return false;
        }

        feature.geometryType =
            GeometryType::MultiLineString;

        return true;
    }

    feature.geometryType =
        GeometryType::Unsupported;

    return false;
}


[[nodiscard]]
bool
parseFeature(
    const JsonValue& featureValue,
    ParsedFeature& feature)
{
    if (!featureValue.isObject()) {
        return false;
    }

    const auto* properties =
        featureValue.find("properties");

    const auto* geometry =
        featureValue.find("geometry");

    if (properties == nullptr ||
        !properties->isObject() ||
        geometry == nullptr ||
        geometry->isNull()) {
        return false;
    }

    feature.highway =
        readStringProperty(
            *properties,
            "highway");

    if (feature.highway.empty()) {
        return false;
    }

    return parseGeometry(
        *geometry,
        feature);
}

} // namespace


std::vector<ParsedFeature>
GeoJSONParser::parse(
    const std::filesystem::path& file) const
{
    const std::string content =
        readFile(file);

    JsonParser parser(content);

    const JsonValue root =
        parser.parse();

    if (!root.isObject()) {
        throw std::runtime_error(
            "GeoJSONParser: root must be an object");
    }

    const auto* type =
        root.find("type");

    if (type == nullptr ||
        !type->isString() ||
        type->string() != "FeatureCollection") {
        throw std::runtime_error(
            "GeoJSONParser: root is not a FeatureCollection");
    }

    const auto* features =
        root.find("features");

    if (features == nullptr ||
        !features->isArray()) {
        throw std::runtime_error(
            "GeoJSONParser: FeatureCollection has no valid features array");
    }

    std::vector<ParsedFeature> result;

    result.reserve(
        features->array().size());

    for (const auto& featureValue :
         features->array()) {
        ParsedFeature feature;

        if (parseFeature(
                featureValue,
                feature)) {
            result.push_back(
                std::move(feature));
        }
    }

    return result;
}

} // namespace persistence::internal
