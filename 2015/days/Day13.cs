namespace aoc2015.days;

public class Day13 : IDay
{
    public uint Day => 13;

    private static List<List<int>> GeneratePermutations(List<int> arrangement)
    {
        var res = new List<List<int>>();

        RecursionHelper(0);
        return res;

        void RecursionHelper(int start)
        {
            if (start == arrangement.Count)
            {
                res.Add([..arrangement]);
                return;
            }

            for (var i = start; i < arrangement.Count; i++)
            {
                (arrangement[start], arrangement[i]) = (arrangement[i], arrangement[start]);
                RecursionHelper(start + 1);
                (arrangement[start], arrangement[i]) = (arrangement[i], arrangement[start]);
            }
        }
    }

    private static int EvaluateArrangement(int[,] units, int[] arrangement)
    {
        var happiness = 0;

        for (var i = 0; i < arrangement.Length; i++)
        {
            var ln = i == 0 ? arrangement[^1] : arrangement[i - 1];
            var rn = (i == (arrangement.Length - 1)) ? arrangement[0] : arrangement[i + 1];

            happiness += units[arrangement[i], ln];
            happiness += units[arrangement[i], rn];
        }

        return happiness;
    }

    public void Run()
    {
        var inp = Utils.GetDayInput(this);

        var extract = (string line) =>
        {
            line = line.Replace("happiness units by sitting next to", string.Empty).Replace("would", string.Empty)
                .Replace(".", string.Empty)
                .Trim();
            var delta = 1;
            if (line.Contains("lose"))
            {
                delta = -1;
                line = line.Replace("lose", string.Empty);
            }
            else
            {
                line = line.Replace("gain", string.Empty);
            }

            var s = line.Split(" ", StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries)
                .Select(q => q.Trim()).ToArray();

            return (s[0], delta * int.Parse(s[1]), s[2]);
        };

        var tups = inp.Split("\n").Select(extract).ToList();

        var pep = new Dictionary<string, int>();
        tups.ForEach(p =>
        {
            if (!pep.ContainsKey(p.Item1))
            {
                pep.Add(p.Item1, pep.Count);
            }
        });

        // pt2
        pep.Add("me", pep.Count);

        Console.WriteLine($"{pep.Count} people");

        var units = new int[pep.Count, pep.Count];
        tups.ForEach((t) => units[pep[t.Item1], pep[t.Item3]] = t.Item2);

        // pt2
        for (var idx = 0; idx < pep.Count; idx++)
        {
            units[pep["me"], idx] = 0;
        }

        List<int> start = [..pep.Values];

        var maxHappy = int.MinValue;

        foreach (var perm in GeneratePermutations(start))
        {
            var c = string.Join(", ", perm);
            var h = EvaluateArrangement(units, perm.ToArray());

            Console.WriteLine($"{c}: {h}");
            maxHappy = Math.Max(maxHappy, h);
        }

        Console.WriteLine($"Maximum Happiness: {maxHappy}");
    }
}
