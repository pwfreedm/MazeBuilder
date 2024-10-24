#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


#include "../include/Maze.hpp"
#include "../include/Cell.hpp"
#include "../include/Wilsons.hpp"
#include "../include/HK.hpp"
#include "Threading.cpp"

namespace py = pybind11;

//TODO: __setitem__ doesn't work b/c the C++ function expects two indices
PYBIND11_MODULE(maze, M)
{
    M.doc() = "A pybind module wrapping a C++ implementation of a Maze";
    
    M.def("parallelize", &parallelize<std::shared_ptr<Cell[]>>);

    py::enum_<Side>(M, "Side")
        .value("L_SIDE", L_SIDE, "left side of a cell (note that cells are actually packed pairs)")
        .value("R_SIDE", R_SIDE, "right side of a cell (note that cells are actually packed pairs)");

    py::class_<Cell>(M, "Cell")
        .def(py::init<>())
        .def("check_direction", &Cell::check_direction)
        .def("up", &Cell::up)
        .def("down", &Cell::down)
        .def("left", &Cell::left)
        .def("right", &Cell::right)
        .def("val", &Cell::val, "get a numeric value of this cells current state")
        .def("compare", &Cell::compare, "compare this cell to another. Returns 1 if this cell has a greater value, -1 if other has a greater value, and 0 if they are equal.")
        .def("setDirection", &Cell::setDirection, "sets the given direction to true")
        .def("updateDirection", &Cell::updateDirection, "flips the state of the given direction")
        .def("__str__", &Cell::str);

    py::class_<Maze<std::shared_ptr<Cell[]>>>(M, "Maze")
        .def(py::init<int, int>())
        .def_property_readonly("width", py::overload_cast<>(&Maze<std::shared_ptr<Cell[]>>::width))
        .def_property_readonly("length", py::overload_cast<>(&Maze<std::shared_ptr<Cell[]>>::length))
        .def("getSide", &Maze<std::shared_ptr<Cell[]>>::getSide)
        .def("__eq__", &Maze<std::shared_ptr<Cell[]>>::operator==)
        .def("__ne__", &Maze<std::shared_ptr<Cell[]>>::operator!=)
        .def("size", py::overload_cast<>(&Maze<std::shared_ptr<Cell[]>>::size))
        .def("__getitem__", &Maze<std::shared_ptr<Cell[]>>::operator[])
        .def("__setitem__", &Maze<std::shared_ptr<Cell[]>>::set)
        .def("connect", &Maze<std::shared_ptr<Cell[]>>::connect)
        .def("validMove", &Maze<std::shared_ptr<Cell[]>>::validMove)
        .def("getNeighbor", &Maze<std::shared_ptr<Cell[]>>::getNeighbor)
        .def("getDirection", &Maze<std::shared_ptr<Cell[]>>::getDirection)
        .def("inMaze", &Maze<std::shared_ptr<Cell[]>>::inMaze)
        .def("hasIndex", &Maze<std::shared_ptr<Cell[]>>::hasIndex)
        .def("__str__", &Maze<std::shared_ptr<Cell[]>>::toString);

    py::class_<Wilsons<std::shared_ptr<Cell[]>>>(M, "Wilsons")
        .def(py::init<Maze<std::shared_ptr<Cell[]>>, int>())
        .def("run", &Wilsons<std::shared_ptr<Cell[]>>::run);

    py::class_<HK<std::shared_ptr<Cell[]>>>(M, "HK")
        .def(py::init<Maze<std::shared_ptr<Cell[]>>, int>())
        .def("run", &HK<std::shared_ptr<Cell[]>>::run);
}