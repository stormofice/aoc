using System.Diagnostics;

namespace aoc2015.days;

public class Day02: IDay
{
    public uint Day => 2;

    public void Run()
    {
        var samples = Utils.GetDaySampleInputs(this);

        var inp = Utils.GetDayInput(this);
        // var inp = samples[0];

        var dims = inp.Split("\n", StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries).Select(line => line.Split("x").Select(int.Parse).ToList());

        var sum = 0;
        var rs = 0;
        foreach (var present in dims)
        {
            Debug.Assert(present.Count == 3);
            var l = present[0];
            var w = present[1];
            var h = present[2];

            present.Sort();

            var ribbon = 2 * present[0] + 2 * present[1] + present.Aggregate(1, (acc, v) => acc * v);
            rs += ribbon;

            var bottom = l * w;
            var longSide = l * h;
            var cap = w * h;

            sum += 2 * bottom + 2 * longSide + 2 * cap + Math.Min(bottom, Math.Min(longSide, cap));
        }

        Console.WriteLine($"Present Paper: {sum}, Ribbon Length: {rs}");
    }
}
