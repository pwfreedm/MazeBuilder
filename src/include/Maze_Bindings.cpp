#include <pybind11/pybind11.h>
#include <vector>

#include "../lib/Maze.cpp"

namespace py = std::pybind11;

PYBIND11_MODULE(Cell, Cell)
{
    Cell.doc() = "A pybind module wrapping a 4-bit cell implementation"
    py::class_<Cell>(Cell, "Cell")
        .def(py::init<bool, bool, bool, bool>())
        .def("val" &Cell::val)
        .def("compare", &Cell::compare)
        .def("setDirection", &Cell::setDirection)
        .def("updateDirection", &Cell::updateDirection)
}


PYBIND11_MODULE(Maze, Maze)
{
    Maze.doc() = "A pybind module wrapping a C++ implementation of a Maze"
    
    py::class_<Maze>(Maze, "Maze")
        .def(py::init<int, int>())
        
    Maze.def()
}