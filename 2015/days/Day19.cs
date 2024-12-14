using System.Diagnostics;

namespace aoc2015.days;

public class Day19 : IDay
{
    public uint Day => 19;

    public void Run()
    {
        // var inp = Utils.GetDaySampleInputs(this)[1];
        var inp = Utils.GetDayInput(this);

        var replacements = new Dictionary<string, HashSet<string>>();

        var target = "?";
        foreach (var line in inp.Split("\n"))
        {
            if (line.Contains("=>"))
            {
                var lr = line.Split(" => ", StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries);
                Trace.Assert(lr.Length == 2);
                if (replacements.TryGetValue(lr[0], out var value))
                {
                    value.Add(lr[1]);
                }
                else
                {
                    replacements.Add(lr[0], [lr[1]]);
                }
            }
            else
            {
                target = line;
            }
        }

        foreach (var (key, value) in replacements)
        {
            Console.WriteLine($"{key} -> {string.Join(",", value)}");
        }

        Console.WriteLine($"Initial: {target}");

        HashSet<string> distincts = [];

        foreach (var (source, repls) in replacements)
        {
            for (var i = 0; i < target.Length; i++)
            {
                if (source.Length == 1)
                {
                    if (target[i] == source[0])
                    {
                        foreach (var repl in repls)
                        {
                            var n = target.Remove(i, 1).Insert(i, repl);
                            distincts.Add(n);
                        }
                    }
                }
                else
                {
                    if (target[i] == source[0] && i != target.Length - 1 && target[i + 1] == source[1])
                    {
                        foreach (var repl in repls)
                        {
                            var n = target.Remove(i, 2).Insert(i, repl);
                            distincts.Add(n);
                        }
                    }
                }
            }
        }

        Console.WriteLine($"Found {distincts.Count} distinct molecules");

        // var sc = Backtrack(replacements, "e", "HOHOHO", 0);
        // var sc = Backtrack(replacements, "e", target, 0);

        var greedy = Backwards(replacements, "e", target);

        Console.WriteLine($"Step count until target: {greedy}");

    }

    private int Backwards(Dictionary<string, HashSet<string>> replacements, string target, string start)
    {
        var reversed = new Dictionary<string, string>();
        foreach (var (key, value) in replacements)
        {
            foreach (var vv in value)
            {
                reversed[vv] = key;
            }
        }

        reversed = reversed.OrderByDescending(kv => kv.Key.Length).ToDictionary();

        var sc = 0;

        while (target != start)
        {
            var p = start;

            foreach (var (repl, src) in reversed)
            {
                var idx = start.IndexOf(repl, StringComparison.Ordinal);
                if (idx < 0)
                    continue;

                p = p.Remove(idx, repl.Length).Insert(idx, src);
                sc++;
                break;
            }

            if (start == p)
            {
                Console.WriteLine("Could not perform replacement?xd");
                break;
            }

            start = p;
        }

        return sc;
    }


    private int Backtrack(Dictionary<string, HashSet<string>> replacements, string current, string goal, int steps)
    {

        if (current == goal)
        {
            Console.WriteLine(steps);
            return steps;
        }

        if (current.Length > goal.Length)
        {
            return int.MaxValue;
        }

        var minSteps = int.MaxValue;

        foreach (var (source, repls) in replacements)
        {
            for (var i = 0; i < current.Length; i++)
            {
                if (source.Length == 1)
                {
                    if (current[i] == source[0])
                    {
                        foreach (var repl in repls)
                        {
                            var n = current.Remove(i, 1).Insert(i, repl);

                            var st = Backtrack(replacements, n, goal, steps + 1);
                            minSteps = Math.Min(st, minSteps);
                        }
                    }
                }
                else
                {
                    if (current[i] == source[0] && i != current.Length - 1 && current[i + 1] == source[1])
                    {
                        foreach (var repl in repls)
                        {
                            var n = current.Remove(i, 2).Insert(i, repl);
                            var st = Backtrack(replacements, n, goal, steps + 1);
                            minSteps = Math.Min(st, minSteps);
                        }
                    }
                }
            }
        }

        return minSteps;

    }

}
