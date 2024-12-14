using System.Diagnostics;
using aoc2015.days;

namespace aoc2015;

public static class Utils
{

    public static string GetDayString(IDay day)
    {
        var date = day.Day;
        return date < 10 ? $"0{date}" : date.ToString();
    }

    public static string GetDayInput(IDay day)
    {
        Debug.Assert(File.Exists($"inputs/day{GetDayString(day)}.txt"));
        return File.ReadAllText($"inputs/day{GetDayString(day)}.txt").Trim();
    }

    public static string[] GetDaySampleInputs(IDay day)
    {
        var values = Directory.EnumerateFiles("inputs/", $"day{GetDayString(day)}.sample*.txt")
            .Select(f => File.ReadAllText(f).Trim())
            .ToArray();
        Debug.Assert(values.Length > 0);
        return values;
    }

}
