/********************************
** Tsunagari Tile Engine       **
** jsons.cpp                   **
** Copyright 2016 Paul Merrill **
********************************/

// **********
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// **********

#define RAPIDJSON_HAS_STDSTRING 1
#define RAPIDJSON_SSE42 1

#include "core/jsons.h"

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

#include "cache/cache-template.cpp"
#include "cache/readercache.h"
#include "core/measure.h"
#include "core/resources.h"

typedef rapidjson::Document RJDocument;
typedef RJDocument::ValueType::ConstObject RJObject;
typedef RJDocument::ValueType::ConstArray RJArray;

class JSONObjectImpl : public JSONObject {
 public:
    virtual ~JSONObjectImpl() = default;

    std::vector<std::string> names() const;

    bool hasBool(const std::string& name) const;
    bool hasInt(const std::string& name) const;
    bool hasUnsigned(const std::string& name) const;
    bool hasDouble(const std::string& name) const;
    bool hasString(const std::string& name) const;
    bool hasObject(const std::string& name) const;
    bool hasArray(const std::string& name) const;

    bool boolAt(const std::string& name) const;
    int intAt(const std::string& name) const;
    unsigned unsignedAt(const std::string& name) const;
    double doubleAt(const std::string& name) const;
    std::string stringAt(const std::string& name) const;
    JSONObjectPtr objectAt(const std::string& name) const;
    JSONArrayPtr arrayAt(const std::string& name) const;

 protected:
    JSONObjectImpl() = default;

    virtual const RJObject& get() const = 0;
};

class JSONObjectReal : public JSONObjectImpl {
 public:
    JSONObjectReal(const RJObject object);

 protected:
    const RJObject& get() const;

 private:
    const RJObject object;
};

class JSONArrayImpl : public JSONArray {
 public:
    explicit JSONArrayImpl(RJArray array);

    size_t size() const;

    bool isBool(size_t index) const;
    bool isInt(size_t index) const;
    bool isUnsigned(size_t index) const;
    bool isDouble(size_t index) const;
    bool isString(size_t index) const;
    bool isObject(size_t index) const;
    bool isArray(size_t index) const;

    bool boolAt(size_t index) const;
    int intAt(size_t index) const;
    unsigned unsignedAt(size_t index) const;
    double doubleAt(size_t index) const;
    std::string stringAt(size_t index) const;
    JSONObjectPtr objectAt(size_t index) const;
    JSONArrayPtr arrayAt(size_t index) const;

 private:
    const RJArray::PlainType& at(size_t index) const;

 private:
    RJArray array;
};


class JSONDocImpl : public JSONObjectImpl {
 public:
    explicit JSONDocImpl(std::string json);

    bool isValid() const;

 protected:
    const RJObject& get() const;

 private:
    std::string json;
    RJDocument document;
    void* object;
    void* objectAddr;
};

class JSONsImpl : public JSONs {
 public:
    JSONsImpl();

    JSONObjectRef load(const std::string& path);

    void garbageCollect();

 private:
    ReaderCache<JSONObjectRef> documents;
};


static JSONsImpl globalJSONs;

JSONs& JSONs::instance() {
    return globalJSONs;
}


std::vector<std::string> JSONObjectImpl::names() const {
    std::vector<std::string> names;
    for (auto& property : get()) {
        auto& name = property.name;
        if (name.IsString()) {
            names.push_back(name.GetString());
        }
    }
    return names;
}

bool JSONObjectImpl::hasBool(const std::string& name) const { return get().HasMember(name) && get()[name].IsBool(); }
bool JSONObjectImpl::hasInt(const std::string& name) const { return get().HasMember(name) && get()[name].IsInt(); }
bool JSONObjectImpl::hasUnsigned(const std::string& name) const { return get().HasMember(name) && get()[name].IsUint(); }
bool JSONObjectImpl::hasDouble(const std::string& name) const { return get().HasMember(name) && get()[name].IsDouble(); }
bool JSONObjectImpl::hasString(const std::string& name) const { return get().HasMember(name) && get()[name].IsString(); }
bool JSONObjectImpl::hasObject(const std::string& name) const { return get().HasMember(name) && get()[name].IsObject(); }
bool JSONObjectImpl::hasArray(const std::string& name) const { return get().HasMember(name) && get()[name].IsArray(); }

bool JSONObjectImpl::boolAt(const std::string& name) const { return get()[name].GetBool(); }
int JSONObjectImpl::intAt(const std::string& name) const { return get()[name].GetInt(); }
unsigned JSONObjectImpl::unsignedAt(const std::string& name) const { return get()[name].GetUint(); }
double JSONObjectImpl::doubleAt(const std::string& name) const { return get()[name].GetDouble(); }
std::string JSONObjectImpl::stringAt(const std::string& name) const { return get()[name].GetString(); }
JSONObjectPtr JSONObjectImpl::objectAt(const std::string& name) const { return JSONObjectPtr(new JSONObjectReal(get()[name].GetObject())); }
JSONArrayPtr JSONObjectImpl::arrayAt(const std::string& name) const { return JSONArrayPtr(new JSONArrayImpl(get()[name].GetArray())); }


JSONObjectReal::JSONObjectReal(const RJObject object) : object(std::move(object)) {}

const RJObject& JSONObjectReal::get() const {
    return object;
}


JSONArrayImpl::JSONArrayImpl(RJArray array) : array(std::move(array)) {}

size_t JSONArrayImpl::size() const {
    return array.Size();
}

bool JSONArrayImpl::isBool(size_t index) const { return at(index).IsBool(); }
bool JSONArrayImpl::isInt(size_t index) const { return at(index).IsInt(); }
bool JSONArrayImpl::isUnsigned(size_t index) const { return at(index).IsUint(); }
bool JSONArrayImpl::isDouble(size_t index) const { return at(index).IsDouble(); }
bool JSONArrayImpl::isString(size_t index) const { return at(index).IsString(); }
bool JSONArrayImpl::isObject(size_t index) const { return at(index).IsObject(); }
bool JSONArrayImpl::isArray(size_t index) const { return at(index).IsArray(); }

bool JSONArrayImpl::boolAt(size_t index) const { return at(index).GetBool(); }
int JSONArrayImpl::intAt(size_t index) const { return at(index).GetInt(); }
unsigned JSONArrayImpl::unsignedAt(size_t index) const { return at(index).GetUint(); }
double JSONArrayImpl::doubleAt(size_t index) const { return at(index).GetDouble(); }
std::string JSONArrayImpl::stringAt(size_t index) const { return at(index).GetString(); }
JSONObjectPtr JSONArrayImpl::objectAt(size_t index) const { return JSONObjectPtr(new JSONObjectReal(at(index).GetObject())); }
JSONArrayPtr JSONArrayImpl::arrayAt(size_t index) const { return JSONArrayPtr(new JSONArrayImpl(at(index).GetArray())); }

const RJArray::PlainType& JSONArrayImpl::at(size_t index) const {
    return array[static_cast<unsigned>(index)];
};


JSONDocImpl::JSONDocImpl(std::string json)
        : json(std::move(json)) {
    document.ParseInsitu<
            rapidjson::kParseInsituFlag |
            rapidjson::kParseCommentsFlag |
            rapidjson::kParseTrailingCommasFlag>(
                const_cast<char*>(this->json.c_str()));
    object = &document;
    objectAddr = &object;
}

bool JSONDocImpl::isValid() const {
    return !document.HasParseError() && document.IsObject();
}

const RJObject& JSONDocImpl::get() const {
    return (RJObject&)object;
}


static JSONObjectRef genJSON(const std::string& path) {
    std::unique_ptr<Resource> r = Resources::instance().load(path);
    if (!r) {
        return nullptr;
    }
    std::string json = r->asString();

    TimeMeasure m("Constructed " + path + " as json");

    std::shared_ptr<JSONDocImpl> document = std::make_shared<JSONDocImpl>(std::move(json));

    return document->isValid() ? document : nullptr;
}

JSONsImpl::JSONsImpl() : documents(genJSON) {}

JSONObjectRef JSONsImpl::load(const std::string& path) {
    return documents.lifetimeRequest(path);
}

void JSONsImpl::garbageCollect() {
    documents.garbageCollect();
}
