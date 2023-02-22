using AutoMapper;
using ICSProject.DAL.Entities;

using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Windows;
using System;
using ICSProject.BL.Models;

namespace ICSProject.App.Wrappers
{
    public class CarWrapper : ModelWrapper<CarDetailModel>
    {
        public CarWrapper(CarDetailModel model) : base(model) { }

        public string? Manufacturer
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public string? Type
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public DateTime? RegistrationTime
        {
            get => GetValue<DateTime>();
            set => SetValue(value);
        }
        public uint? NumberOfSeats
        {
            get => GetValue<uint>();
            set => SetValue(value);
        }
        public Guid? OwnerId
        {
            get => GetValue<Guid>();
            set => SetValue(value);
        }
        public string? ImageUrl
        {
            get => GetValue<string>();
            set => SetValue(value);
        }


       /* public override IEnumerable<ValidationResult> Validate(ValidationContext validationContext)
        {
            if (string.IsNullOrWhiteSpace(Name))
            {
                yield return new ValidationResult($"{nameof(Name)} is required", new[] { nameof(Name) });
            }

            if (string.IsNullOrWhiteSpace(Description))
            {
                yield return new ValidationResult($"{nameof(Description)} is required", new[] { nameof(Description) });
            }
        }*/

        public static implicit operator CarWrapper(CarDetailModel detailModel)
            => new(detailModel);

        public static implicit operator CarDetailModel(CarWrapper wrapper)
            => wrapper.Model;
    }
}