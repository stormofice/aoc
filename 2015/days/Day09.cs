using System.Diagnostics;

namespace aoc2015.days;

public class Day09: IDay
{
    public uint Day => 9;

    private record class Node(string Name, List<(Node, int)> Neighbors);

    public void Run()
    {
        var nd = new Dictionary<string, Node>();

        // var inp = Utils.GetDaySampleInputs(this).First().Split("\n");
        var inp = Utils.GetDayInput(this).Split("\n");

        foreach (var line in inp)
        {
            var lrs = line.Split("=").Select(s => s.Trim()).ToList();
            Trace.Assert(lrs.Count == 2);
            var ls = lrs[0].Split("to").Select(s => s.Trim()).ToList();
            var d = int.Parse(lrs[1]);

            var source = ls[0];
            var dest = ls[1];

            if (!nd.TryGetValue(source, out var srcN))
            {
                nd.Add(source, new(source, []));
                srcN = nd[source];
            }

            if (!nd.TryGetValue(dest, out var destN))
            {
                nd.Add(dest, new(dest, []));
                destN = nd[dest];
            }

            srcN.Neighbors.Add((destN, d));
            destN.Neighbors.Add((srcN, d));
        }


        var m = int.MaxValue;
        var lv = int.MinValue;

        foreach (var (_, node) in nd)
        {
            // try starting at each node
            HashSet<Node> vis = [node];
            var shortest = ShortestPath(vis, node, 0);
            m = Math.Min(m, shortest);
            var highest = LongestPath(vis, node, 0);
            lv = Math.Max(lv, highest);
            Console.WriteLine($"{node.Name}: {shortest} {highest}");
        }

        Console.WriteLine($"Min: {m}, Max: {lv}");
    }

    private static int ShortestPath(HashSet<Node> vis, Node cur, int acc)
    {
        var lowestTry = int.MaxValue;

        foreach (var (n, dist) in cur.Neighbors)
        {
            if (!vis.Add(n))
                continue;

            var ntry = ShortestPath(vis, n, acc + dist);
            lowestTry = Math.Min(lowestTry, ntry);
            vis.Remove(n);
        }

        return (lowestTry == int.MaxValue ? acc : lowestTry);
    }

    private static int LongestPath(HashSet<Node> vis, Node cur, int acc)
    {
        var highestTry = int.MinValue;

        foreach (var (n, dist) in cur.Neighbors)
        {
            if (!vis.Add(n))
                continue;

            var ntry = LongestPath(vis, n, acc + dist);
            highestTry = Math.Max(highestTry, ntry);
            vis.Remove(n);
        }

        return (highestTry == int.MinValue ? acc : highestTry);
    }

}
