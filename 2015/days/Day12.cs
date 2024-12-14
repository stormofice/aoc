using System.Diagnostics;
using System.Text.Json.Nodes;
using System.Text.RegularExpressions;

namespace aoc2015.days;

public class Day12: IDay
{
    public uint Day => 12;
    public void Run()
    {
        var regex = new Regex(@"-?\d+", RegexOptions.Compiled);

        var inp = Utils.GetDayInput(this);

        var sum = 0;

        foreach (Match match in regex.Matches(inp))
        {
            sum += int.Parse(match.Value);
        }

        Console.WriteLine($"Sum: {sum}");

        var root = JsonNode.Parse(inp)!;
        Trace.Assert(root is JsonObject);
        var rootObj = (JsonObject) root;

        var wl = new Queue<JsonNode>();
        wl.Enqueue(rootObj);

        var ms = 0;

        while (wl.Count > 0)
        {
            var cur = wl.Dequeue();

            switch (cur)
            {
                case JsonArray jsonArray:
                    foreach (var jsonNode in jsonArray)
                    {
                        wl.Enqueue(jsonNode!);
                    }
                    break;
                case JsonObject jsonObject:
                    List<JsonNode> wants = [];
                    var hasRed = false;
                    foreach (var (_, value) in jsonObject)
                    {
                        if (value is JsonValue jv)
                        {
                            if (jv.TryGetValue(out string? v))
                            {
                                if (v == "red")
                                {
                                    hasRed = true;
                                    break;
                                }
                            }
                        }
                        wants.Add(value!);
                    }
                    if(!hasRed)
                        wants.ForEach(w => wl.Enqueue(w));
                    break;
                case JsonValue jsonValue:
                    if (jsonValue.TryGetValue(out int no))
                    {
                        ms += no;
                    }
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(cur));
            }
        }

        Console.WriteLine($"Json Sum: {ms}");
    }
}
