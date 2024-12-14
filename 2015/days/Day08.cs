using System.Diagnostics;

namespace aoc2015.days;

public class Day08 : IDay
{
    private static int StrLen(string s)
    {
        var l = 0;
        for (var i = 1; i < s.Length - 1; i++)
        {
            var ch = s[i];
            var n = s[i + 1];
            if (ch == '\\')
            {
                Trace.Assert(n is '\\' or '\"' or 'x');
                if (n is '\\' or '\"')
                    i++;
                else if (n is 'x')
                {
                    i += 3;
                }
            }

            l++;
        }

        return l;
    }

    private static string Encode(string s) => s.Replace(@"\", @"\\").Replace("\"", "\\\"");

    public uint Day => 8;

    public void Run()
    {
        var inp = Utils.GetDayInput(this).Split("\n");

        var cl = inp.Aggregate(0, (acc, s) => acc + s.Length);
        var ml = inp.Aggregate(0, (acc, s) => acc + StrLen(s));
        var el = inp.Aggregate(0, (acc, s) => acc + Encode(s).Length + 2);

        Console.WriteLine($"Code Repr: {cl}, Mem Repr: {ml}, Encoded: {el} --- {cl - ml} - {el - cl}");
    }
}
