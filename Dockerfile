ENV TINI_VERSION v0.19.0
FROM debian:10-slim AS final
ADD https://github.com/krallin/tini/releases/download/${TINI_VERSION}/tini /tini
RUN chmod +x /tini
ENTRYPOINT ["/tini", "--"]

FROM debian:10 AS build
WORKDIR /src
COPY . .
RUN apt-get update \
    && apt-get install -y build-essential cmake libssl-dev \
    && cmake . \
    && make 

FROM final
WORKDIR /app
COPY --from=build /src/bin/ .
COPY /Configs ./Configs

VOLUME ["/app/Configs"]
CMD ["/app/bot"]
