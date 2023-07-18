#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <tinyxml2.h>

using namespace std;
using namespace tinyxml2;

typedef struct VectorPath {
    string pathData;
    string fillColor;

    VectorPath() {}
    VectorPath(string pathData, string fillColor) : pathData(pathData), fillColor(fillColor) {}
};

typedef struct Group {
    vector<VectorPath> paths;

    Group(vector<VectorPath> paths) : paths(paths) {}
};

typedef struct AndroidVectorDrawable {
    vector<VectorPath> paths;
    vector<Group> groups;
    double height;
    double width;

    AndroidVectorDrawable(vector<VectorPath> paths, vector<Group> groups, double width, double height) 
        : paths(paths), groups(groups), height(height), width(width) {}
};

VectorPath getVectorPathFromNode(XMLElement* item) {
    if (string(item->Name()) == "path") {
        string pathData;
        string fillColor;
        const XMLAttribute* attr = item->FirstAttribute();
        while (attr) {
            string name = attr->Name();
            string value = attr->Value();
            if (name == "android:pathData") {
                pathData = value;
            } else if (name == "android:fillColor" && value[0] == '#') {
                fillColor = value;
            }
            attr = attr->Next();
        }
        if (!pathData.empty()) {
            return VectorPath(pathData, fillColor);
        }
    }
    return VectorPath("", "");
}

AndroidVectorDrawable getDrawable(string source) {
    XMLDocument xml;
    xml.LoadFile(source.c_str());
    XMLElement* vector = xml.FirstChildElement("vector");
    const XMLAttribute* attributes = vector->FirstAttribute();
    XMLNode* children = vector->FirstChild();

    double width = 0;
    double height = 0;
    while (attributes) {
        if (string(attributes->Name()) == "android:viewportHeight") {
            height = stod(attributes->Value());
        } else if (string(attributes->Name()) == "android:viewportWidth") {
            width = stod(attributes->Value());
        }
        attributes = attributes->Next();
    }

    std::vector<VectorPath> paths;
    std::vector<Group> groups;

    XMLElement* item = children->ToElement();
    VectorPath path;
    while (item) {
        if (string(item->Name()) == "group") {
            std::vector<VectorPath> groupPaths;
            XMLElement* child = item->FirstChildElement();
            while (child) {
                path = getVectorPathFromNode(child);
                if (!path.pathData.empty()) {
                    groupPaths.push_back(path);
                }
                child = child->NextSiblingElement();
            }
            if (!groupPaths.empty()) {
                groups.push_back(Group(groupPaths));
            }
        }
        else {
            path = getVectorPathFromNode(item);
            if (!path.pathData.empty()) {
                paths.push_back(path);
            }
        }
        item = item->NextSiblingElement();
    }
    return AndroidVectorDrawable(paths, groups, width, height);
}

bool createSvg(string source, string destination) {
    try {
        AndroidVectorDrawable drawable = getDrawable(source);
        XMLDocument doc;
        XMLElement* svg = doc.NewElement("svg");
        svg->SetAttribute("viewBox", ("0 0 " + to_string(drawable.width) + " " + to_string(drawable.height)).c_str());
        svg->SetAttribute("xmlns", "http://www.w3.org/2000/svg");
        svg->SetAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
        for (Group group : drawable.groups) {
            XMLElement* g = doc.NewElement("g");
            for (VectorPath path : group.paths) {
                XMLElement* child = doc.NewElement("path");
                if (!path.fillColor.empty()) {
                    child->SetAttribute("fill", path.fillColor.c_str());
                }
                child->SetAttribute("d", path.pathData.c_str());
                g->InsertEndChild(child);
            }
            svg->InsertEndChild(g);
        }
        for (VectorPath path : drawable.paths) {
            XMLElement* child = doc.NewElement("path");
            if (!path.fillColor.empty()) {
                child->SetAttribute("fill", path.fillColor.c_str());
            }
            child->SetAttribute("d", path.pathData.c_str());
            svg->InsertEndChild(child);
        }
        doc.InsertEndChild(svg);
        doc.SaveFile(destination.c_str());
        return true;
    } catch (exception& e) {
        cout << "Failed: " << e.what() << endl;
        return false;
    }
}

int main(int argc, char** argv) {
    if (argc < 2 || string(argv[1]) == "--help") {
        cout << "Convert Android VectorDrawable XML resource file to SVG" << endl;
        cout << "Usage: " << argv[0] << " [FILE]..." << endl;
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        string path = argv[i];
        string destination = regex_replace(path, regex("\\.[^.]+$"), ".svg");
        if (!createSvg(path, destination)) {
            cout << "Error creating SVG from " << path << endl;
        }
    }
    return 0;
}
