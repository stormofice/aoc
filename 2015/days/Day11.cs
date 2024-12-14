using System.Diagnostics;

namespace aoc2015.days;

public class Day11 : IDay
{
    public uint Day => 11;

    private void Increment(char[] pw)
    {
        for (var i = pw.Length - 1; i >= 0; i--)
        {
            var ch = pw[i];
            if (ch == 'z')
            {
                if(i == 0)
                    Trace.Assert(false);
                pw[i] = 'a';
            }
            else
            {
                pw[i]++;
                break;
            }
        }
    }

    private bool Valid(char[] pw)
    {
        var straight = false;

        var ps = new Dictionary<char, int>();

        for (var i = 0; i < pw.Length; i+= 2)
        {
            if (i == pw.Length - 1)
                break;

            var ch = pw[i];
            var n = pw[i + 1];
            if (ch == n)
            {
                if (!ps.TryAdd(ch, 1))
                {
                    ps[ch]++;
                }
            }
        }

        for (var i = 1; i < pw.Length; i += 2)
        {
            if (i == pw.Length - 1)
                break;

            var ch = pw[i];
            var n = pw[i + 1];
            if (ch == n)
            {
                if (!ps.TryAdd(ch, 1))
                {
                    ps[ch]++;
                }
            }
        }

        for (var i = 0; i < pw.Length; i++)
        {
            var ch = pw[i];

            if (ch is 'i' or 'o' or 'l')
                return false;

            if (i < pw.Length - 2)
            {
                var n = pw[i + 1];
                var nn = pw[i + 2];
                if ((ch + 2 == nn) && (n + 1 == nn))
                {
                    straight = true;
                }
            }

        }

        var hastp = ps.Count >= 2;

        return straight && hastp;
    }

    public void Run()
    {
        var inp = "hxbxxyzz";
        var ii = inp.ToCharArray();

        var tp = ii;
        do
        {
            Increment(ii);
        } while (!Valid(tp));

        Console.WriteLine(tp);
    }
}
