using System.Text.RegularExpressions;

namespace aoc2015.days;

public class Day15 : IDay
{
    private record class Ingredient(long Capacity, long Durability, long Flavor, long Texture, long Calories);

    public uint Day => 15;

    private static long EvalPicks(List<Ingredient> ingredients, long[] picks)
    {
        long capacity = 0, durability = 0, flavor = 0, texture = 0;
        for (var i = 0; i < picks.Length; i++)
        {
            capacity += picks[i] * ingredients[i].Capacity;
            durability += picks[i] * ingredients[i].Durability;
            flavor += picks[i] * ingredients[i].Flavor;
            texture += picks[i] * ingredients[i].Texture;
        }

        capacity = Math.Max(0, capacity);
        durability = Math.Max(0, durability);
        flavor = Math.Max(0, flavor);
        texture = Math.Max(0, texture);

        return capacity * durability * flavor * texture;
    }

    private static long TotalCals(List<Ingredient> ingredients, long[] picks) =>
        picks.Select((t, i) => t * ingredients[i].Calories).Sum();


    public void Run()
    {
        // var inp = Utils.GetDaySampleInputs(this).First();
        var inp = Utils.GetDayInput(this);

        var noRegex = new Regex(@"-?\d+", RegexOptions.Compiled);

        var f = inp.Split("\n").Select(s => noRegex.Matches(s).Select(v => long.Parse(v.Value)).ToList())
            .Select(v => new Ingredient(v[0], v[1], v[2], v[3], v[4])).ToList();

        f.ForEach(Console.WriteLine);

        var highest = long.MinValue;
        var totalCalScore = long.MinValue;
        for (var i1 = 0; i1 < 100; i1++)
        {
            for (var i2 = 0; i2 < (100 - i1); i2++)
            {
                for (var i3 = 0; i3 < (100 - i1 - i2); i3++)
                {
                    var left = 100 - i1 - i2 - i3;
                    if (left < 0)
                        continue;
                    long[] picks = [i1, i2, i3, left];
                    var tt = EvalPicks(f, picks);

                    highest = Math.Max(highest, tt);

                    var cals = TotalCals(f, picks);
                    if (cals == 500)
                    {
                        totalCalScore = Math.Max(tt, totalCalScore);
                    }
                }
            }
        }

        Console.WriteLine($"Highest Score: {highest}, Highest TotalCalScore: {totalCalScore}");
    }
}
