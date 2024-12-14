using System.Text;

namespace aoc2015.days;

public class Day10: IDay
{
    public uint Day => 10;
    public void Run()
    {
        var inp = "3113322113";

        for (var i = 0; i < 50; i++)
        {
            Console.WriteLine($"Step: {i}");
            inp = Step(inp);
        }

        Console.WriteLine(inp);
        Console.WriteLine(inp.Length);
    }

    private static string Step(string inp)
    {

        List<(int, int)> pairs = [];

        var prev = -1;
        var count = 0;
        var first = true;
        foreach (var ch in inp.Select(c => c - '0'))
        {
            if (prev == ch)
            {
                count++;
            }
            else
            {
                if (!first)
                    pairs.Add((prev, count));

                first = false;
                prev = ch;
                count = 1;
            }
        }

        pairs.Add((prev, count));

        StringBuilder sb = new();

        foreach (var (val, ct) in pairs)
        {
            sb.Append((char) (ct + '0'));
            sb.Append((char) (val + '0'));
        }

        return sb.ToString();
    }

}
