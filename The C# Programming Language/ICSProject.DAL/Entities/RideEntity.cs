namespace ICSProject.DAL.Entities;

public record RideEntity(
    Guid Id,
    DateTime StarTime,
    DateTime EndTime,
    string Start,
    string End,
    uint Price,
    Guid CarId,
    Guid DriverId) : IEntity
{
    //Automapper requires parameter less constructor for collection synchronization for now
#nullable disable
    public RideEntity() : this(default, default, default, default, default, default, default, default) {}
#nullable enable
    public CarEntity? Car { get; init; }
    public UserEntity? Driver { get; init; }
    public ICollection<PassengerRideEntity> PassengerRides { get; init; } = new List<PassengerRideEntity>();
    public ICollection<SemiStopEntity> SemiStops { get; init; } = new List<SemiStopEntity>();
}