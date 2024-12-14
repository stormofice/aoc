using System.Diagnostics;

namespace aoc2015.days;

public class Day03 : IDay
{
    public uint Day => 3;

    public void Run()
    {
        var inp = Utils.GetDayInput(this);

        HashSet<(int, int)> ps = [];

        ps.Add((0, 0));

        int x = 0, y = 0;
        int rx = 0, ry = 0;

        var isSanta = true;

        foreach (var c in inp)
        {
            switch (c)
            {
                case '^':
                    if (isSanta)
                        y--;
                    else
                        ry--;
                    break;
                case 'v':
                    if (isSanta)
                        y++;
                    else
                        ry++;
                    break;
                case '>':
                    if (isSanta)
                        x++;
                    else
                        rx++;
                    break;
                case '<':
                    if (isSanta)
                        x--;
                    else
                        rx--;
                    break;
                default:
                    Debug.Assert(false);
                    break;

            }

            ps.Add(isSanta ? (x, y) : (rx, ry));
            isSanta = !isSanta;
        }

        Console.WriteLine($"Visited {ps.Count} distinct houses");
    }
}
