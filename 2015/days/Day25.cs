namespace aoc2015.days;

public class Day25 : IDay
{
    public uint Day => 25;

    public void Run()
    {
        var grid = new ulong[50000, 50000];

        var previous = (int x, int y) => x == 0 ? (y - 1, 0) : (x - 1, y + 1);

        var path = new List<(int, int)>();

        (var cx, var cy) = (3029, 2947);

        while (!(cx == 0 && cy == 0))
        {
            path.Add((cx, cy));
            var (px, py) = previous(cx, cy);
            cx = px;
            cy = py;
        }

        path.Reverse();

        grid[0, 0] = 20151125;

        int stop = 30;
        foreach (var (x, y) in path)
        {
            var (px, py) = previous(x, y);

            var pv = grid[py, px];

            if(stop -- > 0)
            Console.WriteLine($"At {x} {y}, prev: {px} {py} :: pv: {pv}");

            var v = (pv * 252533) % 33554393;
            grid[y, x] = v;
        }

        Console.WriteLine($"grid[2947, 3029]={grid[2947-1, 3029-1]}");
        Console.WriteLine($"grid[3029, 2947]={grid[3029-1, 2947-1]}");
    }
}
