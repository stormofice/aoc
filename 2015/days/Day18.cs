using System.Diagnostics;
using System.Text;

namespace aoc2015.days;

public class Day18 : IDay
{
    public uint Day => 18;

    private int width = 100;
    private int height = 100;

    private int LightCount(bool[,] lights, int x, int y)
    {
        var lc = 0;
        if (x + 1 < width)
        {
            if (y + 1 < height)
            {
                lc += lights[x + 1, y + 1] ? 1 : 0;
            }

            lc += lights[x + 1, y] ? 1 : 0;

            if (y - 1 >= 0)
            {
                lc += lights[x + 1, y - 1] ? 1 : 0;
            }
        }

        if (x - 1 >= 0)
        {
            if (y + 1 < height)
            {
                lc += lights[x - 1, y + 1] ? 1 : 0;
            }

            lc += lights[x - 1, y] ? 1 : 0;

            if (y - 1 >= 0)
            {
                lc += lights[x - 1, y - 1] ? 1 : 0;
            }
        }

        if (y + 1 < height)
        {
            lc += lights[x, y + 1] ? 1 : 0;
        }

        if (y - 1 >= 0)
        {
            lc += lights[x, y - 1] ? 1 : 0;
        }

        return lc;
    }

    private void PrintLights(bool[,] lights)
    {
        var sb = new StringBuilder();
        for (var y = 0; y < height; y++)
        {
            for (var x = 0; x < width; x++)
            {
                sb.Append(lights[x, y] ? '#' : '.');
            }

            sb.Append(Environment.NewLine);
        }

        Console.WriteLine(sb.ToString());
    }

    public void Run()
    {
        /*
        var input = """
                    .#.#.#
                    ...##.
                    #....#
                    ..#...
                    #.#..#
                    ####..
                    """;*/
        var input = Utils.GetDayInput(this);
        var lines = input.Split("\n");
        height = lines.Length;
        width = lines[0].Length;

        var lights = new bool[width, height];

        for (var y = 0; y < lines.Length; y++)
        {
            for (var x = 0; x < lines[y].Length; x++)
            {
                if (lines[y][x] == '.')
                {
                }
                else if (lines[y][x] == '#')
                {
                    lights[x, y] = true;
                }
                else
                {
                    Trace.Assert(false);
                }
            }
        }

        PrintLights(lights);

        // pt2
        lights[0, 0] = true;
        lights[width - 1, 0] = true;
        lights[0, height - 1] = true;
        lights[width - 1, height - 1] = true;

        PrintLights(lights);

        const int steps = 100;
        for (var i = 0; i < steps; i++)
        {
            var ll = new bool[width, height];

            for (var y = 0; y < height; y++)
            {
                for (var x = 0; x < width; x++)
                {
                    if (y == 0 && x == 0 || y == 0 && x == width - 1 || y == height - 1 && x == 0 ||
                        y == height - 1 && x == width - 1)
                    {
                        ll[x, y] = true;
                        continue;
                    }

                    var on = lights[x, y];
                    var nc = LightCount(lights, x, y);

                    if (on)
                    {
                        ll[x, y] = nc is 2 or 3;
                    }
                    else
                    {
                        ll[x, y] = nc is 3;
                    }
                }
            }

            lights = ll;
        }

        var lc = lights.Cast<bool>().Count(light => light);

        Console.WriteLine($"{lc} lights are on!");
    }
}
