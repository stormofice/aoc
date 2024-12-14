using System.Diagnostics;

namespace aoc2015.days;

internal enum Kind
{
    On,
    Off,
    Toggle,
}

internal record Rectangle(int x, int y, int hx, int hy);

internal record Command(Kind kind, Rectangle rect);

public class Day06 : IDay
{
    public uint Day => 6;

    public void Run()
    {
        var inp = Utils.GetDayInput(this);

        var cmds = new List<Command>();

        foreach (var se in inp.Split("\n"))
        {
            var st = se;
            var k = Kind.Off;
            if (se.StartsWith("turn on"))
            {
                k = Kind.On;
            }
            else if (se.StartsWith("turn off"))
            {
                k = Kind.Off;
            }
            else if (se.StartsWith("toggle"))
            {
                k = Kind.Toggle;
            }
            else
            {
                Debug.Assert(false);
            }

            st = st.Replace("turn on", string.Empty).Replace("turn off", string.Empty).Replace("toggle", string.Empty)
                .Trim();
            var spp = st.Split(" through ");
            var sp1 = spp[0].Split(",").Select(int.Parse).ToArray();
            var sp2 = spp[1].Split(",").Select(int.Parse).ToArray();
            Debug.Assert(sp1.Length == 2 && sp2.Length == 2);

            var rect = new Rectangle(sp1[0], sp1[1], sp2[0], sp2[1]);
            cmds.Add(new(k, rect));
        }

        var lights = new bool[1000, 1000];
        var brights = new int[1000, 1000];

        foreach (var (kind, rect) in cmds)
        {
            for (var x = rect.x; x <= rect.hx; x++)
            {
                for (var y = rect.y; y <= rect.hy; y++)
                {
                    lights[x, y] = kind switch
                    {
                        Kind.On => true,
                        Kind.Off => false,
                        Kind.Toggle => !lights[x, y],
                        _ => throw new ArgumentOutOfRangeException(),
                    };
                    switch (kind)
                    {
                        case Kind.On:
                            brights[x, y]++;
                            break;
                        case Kind.Off:
                            brights[x, y] = Math.Max(0, brights[x, y] - 1);
                            break;
                        case Kind.Toggle:
                            brights[x, y] += 2;
                            break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                }
            }
        }

        var lc = lights.Cast<bool>().Count(light => light);
        var bc = brights.Cast<int>().Sum();

        Console.WriteLine($"{lc} lights are on, brightness {bc}");
    }
}
