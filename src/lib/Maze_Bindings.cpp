#include <pybind11/pybind11.h>

#include "Maze.cpp"
#include "Wilsons.cpp"
#include "HK.cpp"

namespace py = pybind11;


PYBIND11_MODULE(Maze, M)
{
    M.doc() = "A pybind module wrapping a C++ implementation of a Maze";
    
    py::class_<Cell>(M, "Cell")
        .def(py::init<bool, bool, bool, bool>())
        .def("val", &Cell::val, "get a numeric value of this cells current state")
        .def("compare", &Cell::compare, "compare this cell to another. Returns 1 if this cell has a greater value, -1 if other has a greater value, and 0 if they are equal.")
        .def("setDirection", &Cell::setDirection, "sets the given direction to true")
        .def("updateDirection", &Cell::updateDirection, "flips the state of the given direction");

    py::class_<Maze>(M, "Maze")
        .def(py::init<int, int>())
        .def_property_readonly("width", py::overload_cast<>(&Maze::width))
        .def_property_readonly("maze", &Maze::data)
        .def("__eq__", &Maze::operator==)
        .def("__ne__", &Maze::operator!=)
        .def("size", py::overload_cast<>(&Maze::size))
        .def("__getitem__", &Maze::operator[])
        .def("__setitem__", &Maze::set)
        .def("connect", &Maze::connect)
        .def("validMove", &Maze::validMove)
        .def("getNeighbor", &Maze::getNeighbor)
        .def("getDirection", &Maze::getDirection)
        .def("inMaze", &Maze::inMaze)
        .def("hasIndex", &Maze::hasIndex)
        .def("__str__", &Maze::toString);

    py::class_<Wilsons>(M, "Wilsons")
        .def(py::init<Maze, int>())
        .def("run", &Wilsons::run);

    py::class_<HK>(M, "HK")
        .def(py::init<Maze, int>())
        .def("run", &HK::run);
}