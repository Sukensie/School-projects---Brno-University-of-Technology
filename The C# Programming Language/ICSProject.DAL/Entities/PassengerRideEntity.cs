namespace ICSProject.DAL.Entities
{
    public record PassengerRideEntity(
        Guid Id,
        Guid PassengerId,
        Guid RideId) : IEntity
    {
        //Automapper requires parameter less constructor for collection synchronization for now
#nullable disable
        public PassengerRideEntity() : this(default, default, default) {}
#nullable enable
        public UserEntity? Passenger { get; init; }
        public RideEntity? Ride { get; init; }
    }
}
