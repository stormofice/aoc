namespace aoc2015.days;

public class Day05: IDay
{
    public uint Day => 5;

    private static bool IsNice(string s)
    {

        var vowelCount = 0;

        var doubleLetter = false;

        for (var i = 0; i < s.Length; i++)
        {
            var ch = s[i];
            if (ch is 'a' or 'e' or 'o' or 'i' or 'u')
                vowelCount++;

            if (i == s.Length - 1) continue;
            var next = s[i + 1];

            if (ch == next)
                doubleLetter = true;

            var cs = $"{ch}{next}";
            if (cs is "ab" or "cd" or "pq" or "xy")
                return false;
        }

        return vowelCount >= 3 && doubleLetter;
    }

    private static bool IsNice2(string s)
    {
        var hasPair = false;
        var pairs = new Dictionary<string, int>();
        for (var i = 0; i < s.Length - 1; i++)
        {
            var pair = $"{s[i]}{s[i + 1]}";
            if (pairs.TryGetValue(pair, out var pair1))
            {
                if (i - pair1 >= 2)
                {
                    hasPair = true;
                    break;
                }
            }
            else
            {
                pairs[pair] = i;
            }
        }

        if (!hasPair)
            return false;

        for (var i = 1; i < s.Length - 1; i++)
        {
            var p = s[i - 1];
            var n = s[i + 1];
            if (p == n)
            {
                return true;
            }

        }

        return false;
    }

    public void Run()
    {
        var inp = Utils.GetDayInput(this);

        Console.WriteLine(IsNice2("ieodomkazucvgmuy"));

        var c = inp.Split("\n").Count(IsNice);
        var c2 = inp.Split("\n").Count(IsNice2);

        Console.WriteLine($"Nice Strings: {c}, Nice2 Strings: {c2}");
    }
}
