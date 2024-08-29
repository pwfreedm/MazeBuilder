#include <pybind11/pybind11.h>

#include "../Maze.cpp"

namespace py = pybind11;

PYBIND11_MODULE(Cell, C)
{
    C.doc() = "A pybind module wrapping a 4-bit cell implementation";
    py::class_<Cell>(C, "Cell")
        .def(py::init<bool, bool, bool, bool>())
        .def("val", &Cell::val)
        .def("compare", &Cell::compare)
        .def("setDirection", &Cell::setDirection)
        .def("updateDirection", &Cell::updateDirection);
}


PYBIND11_MODULE(Maze, M)
{
    M.doc() = "A pybind module wrapping a C++ implementation of a Maze";
    
    py::class_<Maze>(M, "Maze")
        .def(py::init<int, int>());
}