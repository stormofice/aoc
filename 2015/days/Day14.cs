using System.Diagnostics;
using System.Text.RegularExpressions;

namespace aoc2015.days;

public class Day14: IDay
{

    private record class Reindeer(int Speed, int Duration, int Rest);

    public uint Day => 14;
    public void Run()
    {

        var deers = new List<Reindeer>();

        var noRegex = new Regex(@"\d+", RegexOptions.Compiled);

        // var inp = Utils.GetDaySampleInputs(this).First();
        var inp = Utils.GetDayInput(this);

        foreach (var line in inp.Split("\n"))
        {
            var ms = noRegex.Matches(line).Select(s => int.Parse(s.Value)).ToList();
            Trace.Assert(ms.Count == 3);
            deers.Add(new(ms[0], ms[1], ms[2]));
        }

        var distances = new int[deers.Count];
        var state = new int[deers.Count];
        for (var i = 0; i < deers.Count; i++)
        {
            state[i] = deers[i].Duration;
        }

        var points = new int[deers.Count];

        const int seconds = 2503;
        for (var s = 0; s < seconds; s++)
        {
            for (var i = 0; i < state.Length; i++)
            {
                var deer = deers[i];
                switch (state[i])
                {
                    case > 0:
                        distances[i] += deer.Speed;
                        state[i]--;
                        if (state[i] == 0)
                            state[i] = -deer.Rest;
                        break;
                    case 0:
                        Trace.Assert(false);
                        break;
                    case < 0:
                        state[i]++;
                        if (state[i] == 0)
                            state[i] = deer.Duration;
                        break;
                }
            }

            foreach (var t in distances.Select((d, i) => (d,i)).Where(d => d.d == distances.Max()))
            {
                points[t.i]++;
            }
        }

        for (var i = 0; i < distances.Length; i++)
        {
            Console.WriteLine($"{i}: {distances[i]} {points[i]}");
        }

        Console.WriteLine(distances.Max());
        Console.WriteLine(points.Max());
    }
}
