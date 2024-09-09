#include <pybind11/pybind11.h>

#include "../Maze.cpp"

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
        //how to overload resolve if the only difference is const-ness?
        .def_property_readonly("width", static_cast<void (Maze::*) ()>(&Maze::width))
}