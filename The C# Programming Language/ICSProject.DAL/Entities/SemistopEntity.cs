namespace ICSProject.DAL.Entities;

public record SemiStopEntity(
    Guid Id,
    string City,
    uint Order,
    DateTime ArrivalTime,
    Guid RideId) : IEntity
{

#nullable disable
    public SemiStopEntity() : this(default, default, default, default, default) { }
#nullable enable
    public RideEntity? Ride { get; init; }
}