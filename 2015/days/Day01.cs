namespace aoc2015.days;

public class Day01: IDay
{
    public uint Day => 1;

    public void Run()
    {
        var c = Utils.GetDayInput(this);
        // var c = Utils.GetDaySampleInputs(this)[1];

        var pos = 0;
        var basement_pos = -1;
        var floors = c.Aggregate(0, (acc, fc) =>
        {
            if (acc < 0 || basement_pos != -1)
            {
                basement_pos = pos;
            }
            else
            {
                pos++;
            }
            return acc + (fc == '(' ? 1 : -1);
        });

        Console.WriteLine($"Floor {floors}, Entered Basement at: {basement_pos}");

    }
}
