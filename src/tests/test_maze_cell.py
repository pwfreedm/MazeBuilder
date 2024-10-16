from maze import Maze, Cell

def test_maze_ctor():
    m = Maze(5,5)
    assert m.width() == 5
    assert m.size() == 25
    assert isinstance(m, Maze)

def test_cell_ctor():
    c = Cell(0,1,1,0)
    assert str(c) == "[0,1,1,0]"
    assert isinstance(c, Cell)